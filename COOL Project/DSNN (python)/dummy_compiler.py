import os
import win32pipe
import win32file
import win32api
import winerror
import json
import torch
import win32event


# from trainingProcess import object_to_dict
def object_to_dict(obj):
    # If obj is an object, convert it to a dictionary
    if hasattr(obj, "__dict__"):
        obj_dict = {}
        # For each attribute, recursively convert it to a dictionary if it's an object
        for key, value in obj.__dict__.items():
            obj_dict[object_to_dict(key)] = object_to_dict(value)
        return obj_dict
    # If obj is a list, recursively convert its items
    elif isinstance(obj, list):
        return [object_to_dict(item) for item in obj]
    # If obj is a set, convert it to a list
    elif isinstance(obj, set):
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
            self.handle = self.create_or_connect_pipe(directory_path, file_name_stem)
            self.open_pipe_for_read()
            # print("Timeout or error waiting for the server.")
        else:
            # event = win32event.CreateEvent(
            #     None,
            #     0,
            #     0,
            #     "Global\\" + file_name_stem,
            # )
            self.handle = self.create_or_connect_pipe(directory_path, file_name_stem)
            # win32event.SetEvent(event)
            self.open_pipe_for_write()

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
                win32pipe.WaitNamedPipe(full_pipe_name, win32pipe.NMPWAIT_WAIT_FOREVER)
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
                win32pipe.ConnectNamedPipe(handle, None)
                return handle

    def open_pipe_for_read(self):
        win32pipe.SetNamedPipeHandleState(
            self.handle, win32pipe.PIPE_READMODE_MESSAGE, None, None
        )

    def open_pipe_for_write(self):
        win32pipe.SetNamedPipeHandleState(
            self.handle,
            win32pipe.PIPE_READMODE_MESSAGE | win32pipe.PIPE_ACCESS_INBOUND,
            None,
            None,
        )

    def read_json_message(self):
        if self.mode != "r":
            raise ValueError("Pipe is not in read mode")

        full_message = ""
        while True:
            result, data = win32file.ReadFile(self.handle, 65536)
            if result == 0:
                full_message += data.decode()
                try:
                    return json.loads(full_message)
                except json.JSONDecodeError:
                    continue

    def write_object_message(self, obj):
        if self.mode != "w":
            raise ValueError("Pipe is not in write mode")
        json_message = json.dumps(object_to_dict(obj)) if isinstance(obj,str) == False else obj # if the obj is a string, don't dumps.
        win32file.WriteFile(self.handle, json_message.encode())

    def close_pipe(self):
        win32pipe.DisconnectNamedPipe(self.handle)
        win32file.CloseHandle(self.handle)

    def __del__(self):
        self.close_pipe()

def create_training_msg(training_data_map: list[tuple])->str:
    # Initialize a dictionary to hold the JSON structure
    j = {}
    j["order"] = "train"
    j["training_data"] = []

    # Iterate over the training data map (which is a dictionary in Python)
    for file_class_name, training_data_file_name in training_data_map:
        # Create a dictionary for each pair
        jpair = {
            "file_class_name": file_class_name,
            "training_data_file_name": training_data_file_name
        }
        # Append this dictionary to the training_data list
        j["training_data"].append(jpair)

    # Convert the dictionary to a JSON string and return it
    return json.dumps(j, indent=4)  


if __name__ == "__main__":

    training = True
    predicting = not training

    # Usage:
    coolang_path = os.getenv("COOLANG_HOME")

    pipe_comm1 = PipeCommunication(coolang_path, "order_to_neural_network_wizard.pipe", "w")

    pipe_comm2 = PipeCommunication(coolang_path, "reply_from_neural_network_wizard.pipe", "r")



    while predicting == True:
        grounding_state_str = """
        {"BDDB_value": 122.04764286922972, "advance": 3, "codeTable": [{"boundtfdomain": "", "grounded": false, "operand1": {"argName": "x", "argType": "identifier", "changeable": 1, "className": "", "isClass": 0}, "operand2": {"argName": "2", "argType": "number", "changeable": 0, "className": "", "isClass": 0}, "operator": {"argName": "^", "argType": "other"}, "result": {"argName": "3335.3.6.147.3", "argType": "identifier", "changeable": 1, "className": "", "isClass": 0}, "root": false}, {"boundtfdomain": "", "grounded": false, "operand1": {"argName": "-1", "argType": "number", "changeable": 0, "className": "", "isClass": 0}, "operand2": {"argName": "3335.3.6.147.3", "argType": "identifier", "changeable": 1, "className": "", "isClass": 0}, "operator": {"argName": "*", "argType": "other"}, "result": {"argName": "3335.3.6.149", "argType": "identifier", "changeable": 1, "className": "", "isClass": 0}, "root": false}, {"boundtfdomain": "", "grounded": false, "operand1": {"argName": "x", "argType": "identifier", "changeable": 1, "className": "", "isClass": 0}, "operand2": {"argName": "3335.3.6.149", "argType": "identifier", "changeable": 1, "className": "", "isClass": 0}, "operator": {"argName": "+", "argType": "other"}, "result": {"argName": "3335.3.7", "argType": "identifier", "changeable": 1, "className": "", "isClass": 0}, "root": false}, {"boundtfdomain": "", "grounded": false, "operand1": {"argName": "3335.3.7", "argType": "identifier", "changeable": 1, "className": "", "isClass": 0}, "operand2": {"argName": "-7", "argType": "number", "changeable": 0, "className": "", "isClass": 0}, "operator": {"argName": "+", "argType": "other"}, "result": {"argName": "3335.5", "argType": "identifier", "changeable": 1, "className": "", "isClass": 0}, "root": false}, {"boundtfdomain": "", "grounded": false, "operand1": {"argName": "3335.5", "argType": "identifier", "changeable": 1, "className": "", "isClass": 0}, "operand2": {"argName": "0", "argType": "number", "changeable": 0, "className": "", "isClass": 0}, "operator": {"argName": "==", "argType": "other"}, "result": {"argName": "3336", "argType": "identifier", "changeable": 1, "className": "", "isClass": 0}, "root": false}], "domainstepmap": {"quadratic": 3}, "gstfdomain": "quadratic", "indomain": false, "initialstate": false, "position": 0, "relatedFilesAndClasses": ["quadratic"], "rpsign": true, "rpval": 1.5, "rule": false, "step": 3, "succeed": -1}
        """
        msg_pred = {}
        msg_pred["order"] = 'predict'
        msg_pred["file_class_name"] = ['quadratic', 'family']
        # msg_pred["file_class_name"] = ['test']
        msg_pred["grounding_state"] = json.loads(grounding_state_str)
        pipe_comm1.write_object_message(msg_pred)
        msg_received = pipe_comm2.read_json_message()
        print(msg_received)



    # activate training process
    if (training == True):
        training_data_map = [ # the map must be represented as a list of tuple to be hashable
            #  (['family'], '1__family.json'),
            (['quadratic'], '1__quadratic.json'),
            # (['test'], '1__test.json'),
        ]
        msg_train = create_training_msg(training_data_map)
        print(msg_train)
        pipe_comm1.write_object_message(msg_train)



