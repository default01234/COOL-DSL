import os
import win32pipe
import win32file
import win32api
import winerror
import json
import torch
import win32event
import pywintypes


# from trainingProcess import object_to_dict
def object_to_dict(obj):
    # If obj is an object, convert it to a dictionary
    if hasattr(obj, "__dict__") or isinstance(obj, dict):
        obj_dict = {}
        # For each attribute, recursively convert it to a dictionary if it's an object
        if hasattr(obj, "__dict__"):
            for key, value in obj.__dict__.items():
                obj_dict[object_to_dict(key)] = object_to_dict(value)
        else:  # obj is already a dictionary
            for key, value in obj.items():
                obj_dict[object_to_dict(key)] = object_to_dict(value)
        return obj_dict
    # If obj is a list, recursively convert its items
    elif isinstance(obj, list):
        return [object_to_dict(item) for item in obj]
    # If obj is a set, convert it to a list
    elif isinstance(obj, (set, frozenset)):
        return object_to_dict(list(obj))
    # If obj is a PyTorch Tensor, convert it to a list
    elif isinstance(obj, torch.Tensor):
        return object_to_dict(obj.tolist())
    # Otherwise, return the object as it is
    else:
        return obj


class PipeCommunication:
    def __init__(self, directory_path, file_name_stem, mode):
        self.handle = None
        self.mode = mode
        if self.mode not in ["r", "w"]:
            raise ValueError("Mode should be either 'read' or 'write'")

        if self.mode == "r":
            # event = win32event.OpenEvent(
            #     win32event.EVENT_ALL_ACCESS,
            #     False,
            #     "Global\\" + file_name_stem,
            # )

            # # Wait for the server to signal it's ready (with a timeout of 60 seconds)
            # result = win32event.WaitForSingleObject(event, 60000)

            # if result == win32event.WAIT_OBJECT_0:
            # print("Timeout or error waiting for the server.")
            self.handle = self.create_or_connect_pipe(
                directory_path, file_name_stem)
            self.open_pipe_for_read()
        else:
            self.handle = self.create_or_connect_pipe(
                directory_path, file_name_stem)
            self.open_pipe_for_write()

            # event = win32event.CreateEvent(None, 0, 0, "Global\\" + file_name_stem)
            # win32event.SetEvent(event)

    @staticmethod
    def create_or_connect_pipe(directory_path, file_name_stem):
        original_name = directory_path + '/' + file_name_stem
        original_name = original_name.replace(
            '\\', '_').replace('/', '_').replace(':', '_')
        full_pipe_name = '\\\\.\\pipe\\' + original_name
        try:
            handle = win32file.CreateFile(
                full_pipe_name,
                win32file.GENERIC_READ | win32file.GENERIC_WRITE,
                0,
                None,
                win32file.OPEN_EXISTING,
                0,
                None,
            )
            return handle
        except Exception as e:
            if e.winerror == winerror.ERROR_PIPE_BUSY:
                win32pipe.WaitNamedPipe(
                    full_pipe_name, win32pipe.NMPWAIT_WAIT_FOREVER)
            else:
                handle = win32pipe.CreateNamedPipe(
                    full_pipe_name,
                    win32pipe.PIPE_ACCESS_DUPLEX,
                    win32pipe.PIPE_TYPE_MESSAGE
                    | win32pipe.PIPE_READMODE_MESSAGE
                    | win32pipe.PIPE_WAIT,
                    1,
                    65536,
                    65536,
                    300,
                    None,
                )
                # handle = win32pipe.CreateNamedPipe(
                #     full_pipe_name,
                #     win32pipe.PIPE_ACCESS_DUPLEX,
                #     win32pipe.PIPE_TYPE_MESSAGE
                #     | win32pipe.PIPE_READMODE_MESSAGE
                #     | win32pipe.PIPE_WAIT,
                #     1,
                #     1048576,
                #     1048576,
                #     300,
                #     None,
                # )
                win32pipe.ConnectNamedPipe(handle, None)
                return handle

    def open_pipe_for_read(self):
        win32pipe.SetNamedPipeHandleState(
            self.handle, win32pipe.PIPE_READMODE_MESSAGE, None, None
        )

    def open_pipe_for_write(self):
        win32pipe.SetNamedPipeHandleState(
            self.handle, win32pipe.PIPE_READMODE_MESSAGE, None, None
        )

    def read_json_message(self):
        if self.mode != "r":
            raise ValueError("Pipe is not in read mode")

        full_message = ""
        while True:
            try:
                state = win32pipe.GetNamedPipeHandleState(self.handle)
            except pywintypes.error as e:
                print(f"Error checking pipe state: {e}")
                return None  # Return None to indicate an issue with the pipe
            try:
                result, data = win32file.ReadFile(self.handle, 65536)
            except pywintypes.error as e:
                error_code, error_message = e.args
                if error_code == 234:  # ERROR_MORE_DATA
                    # Continue to clean the remaining unread data
                    print("Partial program exceeds max size limit and may fall into cyclic invocations.")
                    self._clean_pipe()
                    j = {}
                    j["order"] = "predict"
                    j["file_class_name"] = []
                    return j
                else:
                    raise
            if result == 234:
                # Continue to clean the remaining unread data
                print("Partial program exceeds max size limit and may fall into cyclic invocations.")
                self._clean_pipe()
                j = {}
                j["order"] = "predict"
                j["file_class_name"] = []
                return j
            if result != 0:
                # Error encountered
                print(f"Error reading from pipe: {result}")
                input("Press Enter to continue...")  # Pause the program
                return None
            if not data:
                # No more data
                continue
            if result == 0:
                full_message += data.decode().replace("\'", "\"")
                try:
                    j = json.loads(full_message)
                    if isinstance(j, str):
                        # Attempt to parse the JSON string again
                        try:
                            j = json.loads(j)
                            return j
                        except json.JSONDecodeError:
                            # If second parse fails, it might still be incomplete
                            continue
                    return j
                except json.JSONDecodeError:
                    continue
                except Exception as e:
                    # Handle any exception (log or print the error if necessary)
                    print("Error reading JSON message:", e)
                    continue

    def write_object_message(self, obj):
        if self.mode != "w":
            raise ValueError("Pipe is not in write mode")

        json_message = json.dumps(object_to_dict(obj))
        win32file.WriteFile(self.handle, json_message.encode())

    def close_pipe(self):
        if self.handle:
            try:
                # Disconnect the pipe (only if it's the server side of the pipe)
                win32pipe.DisconnectNamedPipe(self.handle)
            except Exception as e:
                print(f"Error disconnecting pipe: {e}")

            try:
                # Close the pipe handle
                win32file.CloseHandle(self.handle)
            except Exception as e:
                print(f"Error closing pipe handle: {e}")

            self.handle = None  # Reset handle to None
        # win32pipe.DisconnectNamedPipe(self.handle)
        # win32file.CloseHandle(self.handle)
    def _clean_pipe(self):
        """ Helper function to clean out unread data in the pipe """
        try:
            while True:
                result, _ = win32file.ReadFile(self.handle, 65536)
                if result == 0:  # No more data
                    break
        except pywintypes.error as e:
            error_code, _ = e.args
            if error_code == 109:  # ERROR_BROKEN_PIPE
                print("Pipe has been closed by the other side.")
            elif error_code == 234:  # ERROR_MORE_DATA
                self._clean_pipe(self)  # Keep cleaning until all data is read
            else:
                raise

    def __del__(self):
        self.close_pipe()


# coolang_path = os.getenv("COOLANG_HOME")
# pipe_comm1 = PipeCommunication(coolang_path, "file_name_stem1", "r")
# pipe_comm2 = PipeCommunication(coolang_path, "file_name_stem2", "w")
# while True:
#     message = pipe_comm1.read_json_message()
#     print(message)
#     pipe_comm2.write_object_message(message)
