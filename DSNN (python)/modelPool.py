import os
import torch.nn as nn
import copy
from collections import OrderedDict, deque
from abc import ABC, abstractmethod
import sys
from typing import Union


class ModelWrapper:
    @abstractmethod
    def train(self, trainingDataFilePath: Union[str, list[str]] = None, trainingDataList: list = None) -> dict:
        """Train the model with the provided training data file path/ training data."""
        pass

    @abstractmethod
    def load_model(self, model_path: str):
        """Load a model from the provided model path."""
        pass

    @abstractmethod
    def load_state_dict(self, state_dict):
        """Directly load state dict"""
    @abstractmethod
    def predict(
        self, grounding_state_str: str, predicted_position_output_file: str,**kwargs
    ) -> dict:
        """Predict using the provided grounding state string and return the results as a dict."""
        pass

    @abstractmethod
    def get_state_dict(self):
        """Equivalent to calling model.state_dict(), return state dict"""
        pass

    @abstractmethod
    def save_model(self, model_path: str):
        """Save the current model to the provided model path."""
        pass


class ModelPool:
    def __init__(self, base_model: ModelWrapper, pool_size=1, eviction_length=40, protection_ratio: float = 0.25):
        '''
        Because of experiment platform, we set pool_size = 1, to avoid memory shortage
        '''
        if sys.version_info < (3, 7):
            raise Exception("Please use Python 3.7 or later.")
        if protection_ratio < 0 or protection_ratio >= 1:
            raise ValueError(
                f"protection_ratio should be in range: [0,1), but {protection_ratio}.")
        self.protection_ratio = protection_ratio
        self.models: list[ModelWrapper] = [
            copy.deepcopy(base_model) for _ in range(pool_size)]
        # Stores model key (name) and its index in the models list
        self.model_map = OrderedDict()
        self.usage_history = deque(maxlen=eviction_length)
        self.pool_size = pool_size
        self.eviction_length = eviction_length
        self.access_counts = {}

    def __getitem__(self, model_name) -> ModelWrapper:
        if model_name not in self.model_map:
            self._add_model(model_name=model_name)
            # raise KeyError("Model not in pool")

        self.record_access(model_name)
        return self.models[self.model_map[model_name]]

    def record_access(self, model_name):
        self.usage_history.append(model_name)
        self.access_counts[model_name] = self.access_counts.get(
            model_name, 0) + 1

    def _add_model(self, model_name):
        if len(self.model_map) < self.pool_size:
            # If pool has space, add model
            index = len(self.model_map)
            model_path = os.environ.get(
                "COOLANG_HOME")+"/"+"neuralNetwork" + '/' + "models" + "/" + model_name
            if os.path.exists(model_path):
                self.models[index].load_model(
                    model_path
                )
                self.model_map[model_name] = index
            else:
                self.model_map[model_name] = index
        else:
            # Evict least frequently used model
            evicted_name = self.evict_model()
            index = self.model_map[evicted_name]

            # Remove occurrences of the evicted model name from usage history
            self.usage_history = deque(
                (item for item in self.usage_history if item != evicted_name), maxlen=self.eviction_length)

            # Remove from model_map and access_counts
            del self.model_map[evicted_name]
            del self.access_counts[evicted_name]

            self.models[index].load_model(model_name)
            self.model_map[model_name] = index

    def evict_model(self):
        # Compute access frequency within a range
        recent_usage = list(self.usage_history)[-self.eviction_length:]
        # Assuming the protection term is half the eviction_length
        protected_range = recent_usage[:int(
            self.eviction_length*self.protection_ratio)]
        unprotected_range = recent_usage[int(
            self.eviction_length*self.protection_ratio):]

        access_frequency = {key: unprotected_range.count(
            key) for key in self.model_map if key not in protected_range}
        # all current models are in protect range, evict the last one
        if (access_frequency.__len__() == 0):
            return protected_range[-1]

        least_used_key = min(access_frequency, key=access_frequency.get)

        del self.model_map[least_used_key]
        del self.access_counts[least_used_key]
        return least_used_key  # returning evicted model name

    # def can_be_replaced(self, model_name):
    #     return self.access_counts.get(model_name, 0) >= self.eviction_length

    def __contains__(self, model_name) -> ModelWrapper:
        return self.model_map.__contains__(model_name)
