import argparse
import copy
import json
import os
import random
import numpy as np
import torch
from pipeHandler import PipeCommunication
from pathlib import Path
from typing import Union
import ctypes
from ctypes import wintypes
import win32api
import win32process
import win32con


# for measurement
import time
from datetime import datetime

from guidedReasoningModel import *
from embeddingTrainingData import load_operand_tokenizer, save_operand_tokenizer
from indexer import Indexer
from modelPool import ModelPool
from extractTrainingDataFromJsonFile import initialize_from_file


class Setting:
    def __init__(self):
        self.version = "COOLANG2022"
        self.codeForm = 3
        self.multiFile = 0
        self.matchStateSiloMaxCapacity = 5
        self.codeTableSiloMaxCapacity = 10
        self.maxSwitchTurn = 15
        self.maxBackStep = 10
        self.debugVersion = 1
        self.execute = 1
        self.debugMode = 0
        self.debugInfoOutputPath = (
            "debugInfoOutputPath.pipe"  # debug functionality doesn't work now
        )
        self.debugOrderInputPath = "debugOrderInputPath.pipe"
        self.baseAlgorithm = 0
        self.userPromptEnabled = 1
        self.userPromptAlgorithm = 0
        self.neuralNetworkEnabled = 1
        self.neuralNetworkAlgorithm = 0
        self.train = 1
        self.exportTrainingData = 1
        self.exportSucceedTrainingDataOnly = 1
        self.exportDuplicatedTrainingData = 0
        self.exportPerformance = 1
        self.cosineSimilarityThreshold: 0.8
        self.sampleProportion = 0.5


def load_settings_from_file(file_name="coolang.setting") -> Setting:
    setting_instance = Setting()

    # Check if the file exists
    try:
        with open(file_name, "r") as file:
            lines = file.readlines()
            for line in lines:
                key, value = line.strip().split(":")

                # Check if the value is a digit, if yes, convert to integer
                if value.isdigit():
                    value = int(value)

                # Set the attribute of the instance using the key
                setattr(setting_instance, key, value)
    except FileNotFoundError:
        print(f"{file_name} not found! Using default settings.")

    return setting_instance


class NeuralNetworkWizard:
    runningInfo = dict()
    coolang_path = os.getenv("COOLANG_HOME")

    def __init__(self, setting_file_path="coolang.setting") -> None:
        print(f"loading setting file...")
        self.settings = load_settings_from_file(setting_file_path)
        print("Settings Loaded:")
        print(f"Version: {self.settings.version}")
        print(f"Debug Mode: {self.settings.debugMode}")
        print(f"Neural Network Enabled: {self.settings.neuralNetworkEnabled}")
        print(f"User Prompt Enabled:{self.settings.userPromptEnabled}")
        print(f"Export Performance:{self.settings.exportPerformance}")

        self.pipeCommunicationInitialization()
        if self.settings.neuralNetworkEnabled:
            self.model_cache = {}  # Dictionary to cache models and their paths
            self.newralNetworkInitialization()
            ...
        pass
        if self.settings.train:
            self.trainingProcessInitialization()

    def pipeCommunicationInitialization(self):
        """
        1 connect to reading and writing pipes
        """
        print(f"pipe connecting...")

        print(f"connecting to reading pipe...")
        time_stamp1 = time.perf_counter()
        self.pipe_reading_handler = PipeCommunication(
            self.coolang_path, "order_to_neural_network_wizard.pipe", "r"
        )
        time_stamp2 = time.perf_counter()
        self.runningInfo["pipe_reading initialization time"] = time_stamp2 - time_stamp1
        print(f"connecting to reading pipe:{True}")

        print(f"connecting to writing pipe...")

        self.pipe_writing_handler = PipeCommunication(
            self.coolang_path, "reply_from_neural_network_wizard.pipe", "w"
        )
        time_stamp3 = time.perf_counter()
        self.runningInfo["pipe_writing initialization time"] = time_stamp3 - time_stamp2
        print(f"connecting to writing pipe:{True}")
        print(f"pipe connected:{True}")

    def newralNetworkInitialization(self):
        """
        1 load index of model and tokenizer
        2 initialize model based on nn algorithm in nn
        """
        print(f"initializing neural network...")
        print(f"loading model indices...")
        self.model_indexer = Indexer(
            index_path=self.coolang_path + '/' + "neuralNetwork" + '/' + "model_index.json"
        )
        print(f"model indices loaded")

        print(f"loading tokenizer...")
        load_operand_tokenizer(
            self.coolang_path + '/' + "neuralNetwork" + '/' + "operand_tokenizer.json"
        )
        print(f"tokenizer loaded")

        if self.settings.neuralNetworkAlgorithm == 0:
            print(f"initializing DSNN model...")
            start_time = time.perf_counter()
            self.model = GuidedReasoning()
            self.model_path:str = ''
            end_time = time.perf_counter()
            self.runningInfo["DSNN initialization time"] = end_time - start_time
            print(f"DSNN model initialized")

            # print(f"initializing model_pool...")
            # start_time = time.perf_counter()
            # self.model_pool = ModelPool(base_model=self.model) there are problems here
            # end_time = time.perf_counter()
            # self.runningInfo["model pool initialization time"] = end_time - start_time
            # print(f"model pool initialized")

        print(f"neural network initialized")
        pass

    def trainingProcessInitialization(self):
        """
        1 load training data indexer
        """
        print(f"initializing training process...")
        print(f"loading training data indices...")
        self.training_data_indexer = Indexer(
            index_path=self.coolang_path + '/' +
            "neuralNetwork" + '/' + "training_data_index.json"
        )
        print(f"training data indices loaded")

        print(f"training process initialized ")

    def readMessage(self) -> dict:
        msg: dict = self.pipe_reading_handler.read_json_message()
        if msg == None:
            print(f"Communication break")
            return None
        if "order" not in msg:
            raise KeyError("Not standard message:{msg}")
        return msg

    def sendMessage(self, msg: dict):
        self.pipe_writing_handler.write_object_message(msg)

    def communicationProcessing(self):
        print(f"Processing communication...")
        self.runningInfo["total_number_of_prediction_order"] = 0
        self.runningInfo["total_number_of_network_invocation"] = 0
        self.runningInfo["total_predicting_time"] = 0
        self.runningInfo["total_number_of_filtered_incorrect_prediction"] = 0
        while True:
            msg = None
            try:
                msg: dict = self.readMessage()
            except Exception as e:
                print(f"An error occurred while reading message: {e}")
                msg = None
            if msg == None:
                print(f"Ending....")
                break

            if msg["order"] == "predict":
                print(f"Predicting....:msg:{json.dumps(msg)}")
                time_stamp1 = time.perf_counter()
                target_file_class_name: set = set(msg["file_class_name"])
                print(json.dumps(msg["file_class_name"]))
                if target_file_class_name != {"built-in"}:
                    target_file_class_name.__len__()
                    pass
                model_items = self.model_indexer.get_values_noncompulsive_subset(
                    target_file_class_name
                )

                model_accuracy = 0
                coverage_rate = 1  # in domain likelyhold
                # advance = 0
                # rpsign = 0
                prediction_list = []
                # model_items:[(diff_key set, best_key set, best_key->data),(),...]

                # calculate the weighted prediction prob
                # prediction_prob = np.array([])#in this experiment, we don't need to weigh the probs of different nn.
                for item in model_items:
                    if item[1] == None or item[2] == None:
                        # this knowledge domain doesn't have a corresponding model
                        continue
                    nn_knowledge_domain = next(iter(item[1]))
                    model_name = item[2]["model_name"]
                    model_accuracy = min(item[2]["policies_accuracy"],item[2]["advance_accuracy"],\
                                         item[2]["rpsign_accuracy"],item[2]["rpval_accuracy"], item[2]["rule_accuracy"])
                    
                    print(f'Loading Model...')
                    if (Path(self.model_path) != Path(
                        self.coolang_path + '/' +
                            "neuralNetwork" + '/' + "models" + '/' + model_name)):
                        self.model_path = str(Path(
                            self.coolang_path + '/' +
                            "neuralNetwork" + '/' + "models" + '/' + model_name))
                        # Check if the model path is already cached
                        if self.model_path in self.model_cache:
                            self.model = self.model_cache[self.model_path]
                            print(f'Model loaded from cache.')
                        else:
                            self.model.load_model(self.model_path)
                            self.model_cache[self.model_path] = copy.deepcopy(self.model)
                            print(f'Model loaded from path.')
                    print(f'Model loaded.')

                    print(f'Predicting...')
                    prediction = {}
                    try:
                        prediction = self.model.predict(
                        grounding_state_str=json.dumps(
                            msg["grounding_state"]), predicted_position_output_file="",nn_knowledge_domain = nn_knowledge_domain
                    )
                    except:
                        print(f'Error in prediction, maybe the code generated by the compiler is obviously wrong : msg:{json.dumps(msg)}')
                        prediction = {}
                        prediction['indomain'] = -1
                        prediction['ac_indomain'] = 0
                        prediction['succeed'] = -1
                        prediction['ac_succeed'] = 0
                        prediction['policies'] = -1
                        prediction['ac_policies'] = 0
                        prediction['advance'] = -1
                        prediction['ac_advance'] = 0
                        prediction['rpsign'] = 0
                        prediction['ac_rpsign'] = 0
                        prediction['rpval'] = 0
                        prediction['ac_rpval'] = 0
                        prediction['rule'] = -1
                        prediction['ac_rule'] = 0
                    
                    prediction['file_class_name'] = item[1]
                    if "advance" not in prediction:
                        prediction["advance"] = 0
                        prediction["ac_advance"] = 0
                    if "rpsign" not in prediction:
                        prediction["rpsign"] = 0
                        prediction["ac_rpsign"] = 0
                    if "rpval" not in prediction:
                        prediction["rpval"] = 0
                        prediction["ac_rpval"] = 0
                    if "rule" not in prediction:
                        prediction["rule"] = 0
                        prediction["ac_rule"] = 0
                    if "policies" not in prediction:
                        prediction["policies"] = 0
                        prediction["ac_policies"] = 0

                    prediction["ac"] = model_accuracy
                    prediction["ci"] = coverage_rate
                    prediction_list.append(prediction)
                    
                    time_stamp2 = time.perf_counter()
                    # record running info
                    self.runningInfo["total_number_of_network_invocation"] += 1
                    if(prediction['indomain'] == -1):
                        self.runningInfo["total_number_of_filtered_incorrect_prediction"] += 1
                    self.runningInfo["total_predicting_time"] += time_stamp2 - time_stamp1
                self.runningInfo["total_number_of_prediction_order"] += 1

                print("Predicting finished, replying prediction result...")
                reply_msg = {'prediction': prediction_list}
                print(reply_msg)
                self.pipe_writing_handler.write_object_message(reply_msg)
                print(f"Replying finished")
            elif msg["order"] == "train":
                print(f"Handling training order....{json.dumps(msg)}")
                print(f"Closing pipe...")
                self.pipe_reading_handler.close_pipe()
                self.pipe_writing_handler.close_pipe()
                print(f"Pipe closed")
                torch.cuda.empty_cache()
                time_stamp0 = time.perf_counter()
                new_training_data_list: list[tuple] = msg[
                    "training_data"
                ]

                for i, new_training_data_item in enumerate(new_training_data_list):
                    self.runningInfo[f"training_turn{i}"] = {}
                    model_items = self.model_indexer.get_values_subset(
                        new_training_data_item["file_class_name"]
                    )
                    current_datetime = datetime.now()
                    train_model_name = (
                        # str(current_datetime.strftime("%y%m%d%H%M%S%f"))
                        # + "_"
                        # + str(self.settings.neuralNetworkAlgorithm)
                        # + "_"
                        # + 
                        new_training_data_item["file_class_name"][0]
                        + ".mdl"
                    )
                    if len(model_items) != 0 and model_items[0][2]!= None and model_items[0][2].get("model_name",None) != None:
                        train_model_name = model_items[0][2]["model_name"]
                        tmp_train_model_path = Path(self.coolang_path) / "neuralNetwork" / "models" / train_model_name
                        print(f"Loading pretrained networks for training....")
                        self.model.load_model(
                            str(tmp_train_model_path)
                        )
                    else:
                        """
                        No trained model found
                        """
                        print(f"No pretrained networks have been found, using blank networks....")
                        self.model = GuidedReasoning()
                        pass

                    print(f"Training process initialized....")
                    print("Training...")
                    time_stamp3 = time.perf_counter()
                    indomain_accuracy, succeed_accuracy, policies_accuracy, advance_accuracy, rpsign_accuracy, rpval_accuracy, rule_accuracy = self.model.train(
                        self.coolang_path
                        + '/' +
                        new_training_data_item["training_data_file_name"]
                    )
                    time_stamp4 = time.perf_counter()
                    self.runningInfo[f"training_turn{i}"]["training_time"] = (
                        time_stamp4 - time_stamp3
                    )
                    self.runningInfo[f"training_turn{i}"]["indomain_accuracy"] = indomain_accuracy
                    self.runningInfo[f"training_turn{i}"]["succeed_accuracy"] = succeed_accuracy
                    self.runningInfo[f"training_turn{i}"]["policies_accuracy"] = policies_accuracy
                    self.runningInfo[f"training_turn{i}"]["advance_accuracy"] = advance_accuracy
                    self.runningInfo[f"training_turn{i}"]["rpsign_accuracy"] = rpsign_accuracy
                    self.runningInfo[f"training_turn{i}"]["rpval_accuracy"] = rpval_accuracy
                    self.runningInfo[f"training_turn{i}"]["rule_accuracy"] = rule_accuracy
                    self.runningInfo[f"training_turn{i}"]["incorrect_preds_filtered"] = self.model.incorrect_preds_filtered
                    print("Training finished")

                    print("Saving model...")
                    new_model_name = ""
                    if model_items.__len__() == 1 and (
                        model_items[0] == None or model_items[0][0].__len__() == 0
                    ):
                        """
                        If we find a model that calls exactly the same library, overwrite it
                        """

                        new_model_name = train_model_name

                    else:
                        """
                        write as new model file
                        """
                        new_model_name = (
                            # str(current_datetime.strftime("%y%m%d%H%M%S%f"))
                            # + "_"
                            # + str(self.settings.neuralNetworkAlgorithm)
                            # + "_"
                            # + 
                            new_training_data_item["file_class_name"][0]
                            + ".mdl"
                        )
                    self.model.save_model(
                        self.coolang_path + '/' + "neuralNetwork" +
                        '/' + "models" + '/' + new_model_name
                    )

                    print("Model saved")

                    print("adding new indices...")
                    self.training_data_indexer.insert_without_overwriting(
                        new_training_data_item["file_class_name"],
                        new_training_data_item["training_data_file_name"],
                    )
                    self.model_indexer.insert(
                        new_training_data_item["file_class_name"],
                        {
                            "model_name": new_model_name,
                            "indomain_accuracy": indomain_accuracy,
                            "succeed_accuracy": succeed_accuracy,
                            "policies_accuracy": policies_accuracy,
                            "advance_accuracy": advance_accuracy,
                            "rpsign_accuracy": rpsign_accuracy,
                            "rpval_accuracy": rpval_accuracy,
                            "rule_accuracy": rule_accuracy,
                        },
                    )
                    print("new indices added")
                time_stamp5 = time.perf_counter()
                self.runningInfo["total_training_time"] = time_stamp5 - time_stamp0

                print("Saving index files....")
                model_index_dict = self.model_indexer.serializable_data()
                with open(
                    self.coolang_path + '/' + "neuralNetwork" + '/' + "model_index.json", "w"
                ) as file:
                    file.write(json.dumps(model_index_dict))

                print("Index files saved")

                

                print("Ending...")
                break

            elif ...:
                ...
        if (self.settings.exportPerformance):
                    print("Exporting performance data...")
                    print(json.dumps(self.runningInfo))
                    
                    current_datetime = datetime.now()
                    log_file_name = (
                        str(current_datetime.strftime("%y%m%d%H%M%S%f"))
                        + "_nn"
                        + str(self.settings.neuralNetworkAlgorithm)
                        + "_filter" + str(self.model.filter)
                        + ".log"
                    )
                    log_directory = self.coolang_path + '/' + "nnPerformanceLog"
                    # Check if the directory exists
                    if not os.path.exists(log_directory):
                        # Create the directory if it does not exist
                        os.makedirs(log_directory)
                    with open(log_directory + '/' + log_file_name, 'w') as log_file:
                        json.dump(self.runningInfo, log_file)
                    print("Performance data exported")


    def _get_cosine_similarity(self, model_name1, model_name2):
        # Extract the state dictionaries for both models
        state_dict1 = self.model_pool[model_name1].get_state_dict()
        state_dict2 = self.model_pool[model_name2].get_state_dict()

        # Flatten the weights of the models
        weights1 = torch.cat([v.flatten() for v in state_dict1.values()])
        weights2 = torch.cat([v.flatten() for v in state_dict2.values()])

        # Compute the cosine similarity between the two weight vectors
        cosine_sim = torch.nn.functional.cosine_similarity(
            weights1.unsqueeze(0), weights2.unsqueeze(0), dim=1
        )

        return cosine_sim.item()

    def _merge_models(self, model_items: list) -> str:
        def parse_triple(triple):
            weight = triple[1].__len__()
            model_name = triple[2]["model_name"]
            return weight, model_name

        model_pool = self.model_pool
        threshold = self.settings.cosineSimilarityThreshold
        # Extract the weight and model_name of the model with the highest weight
        highest_weight, highest_weight_model_name = parse_triple(
            model_items[0])

        # Collect model names and their weights with a cosine similarity above the threshold
        # to the model with the highest weight
        selected_models = [
            (weight, model_name)
            for weight, model_name in [parse_triple(item) for item in model_items[1::]]
            if self._get_cosine_similarity(highest_weight_model_name, model_name)
            > threshold
        ]

        # Include the highest weight model in our selected models
        highest_weight_model = (highest_weight, highest_weight_model_name)
        selected_models.append(highest_weight_model)

        # Initializing a state_dict for the merged/fused model,
        # with the same keys as the state_dict of any model but with all values set to zero tensors
        fused_state_dict = {
            k: torch.zeros_like(v)
            for k, v in model_pool[highest_weight_model_name].get_state_dict().items()
        }

        # Weighted merge of the models
        total_weight = sum([weight_ for weight_, _ in selected_models])
        for weight, model_name in selected_models:
            for key, value in model_pool[model_name].get_state_dict().items():
                fused_state_dict[key] += (weight / total_weight) * value

        # Load the merged state_dict into the model with the highest weight
        model_pool[highest_weight_model_name].load_state_dict(fused_state_dict)

        # Return the name of the model with the merged state_dict
        return highest_weight_model_name


def main(setting_file_path):
    settings = load_settings_from_file(setting_file_path)

    # Print some settings as a demonstration of successful loading
    if settings.neuralNetworkEnabled == False and settings.train == False:
        return False
    else:
        wizard = NeuralNetworkWizard(setting_file_path=setting_file_path)
        wizard.communicationProcessing()


if __name__ == "__main__":
    # os.environ['CUDA_LAUNCH_BLOCKING'] = "1"
        # Define constants
    THREAD_PRIORITY_TIME_CRITICAL = 15

    try:
        pid = win32api.GetCurrentProcessId()
        handle = win32api.OpenProcess(win32con.PROCESS_ALL_ACCESS, True, pid)
        win32process.SetPriorityClass(handle, win32process.REALTIME_PRIORITY_CLASS)
        print("Main thread priority set to time-critical successfully.")
    except Exception as e:
        print(f"Failed to set main thread priority. Error: {e}")

    parser = argparse.ArgumentParser(description="Load settings from a file.")
    parser.add_argument(
        "--setting_file_path",
        type=str,
        help="Absolute path to the setting file.",
        default="coolang.setting",
        nargs="?",
    )

    args = parser.parse_args()

    main(args.setting_file_path)

