import ast
import json
import os


class Indexer:
    def __init__(self, index_path: str = ""):
        self.data = {}
        if os.path.exists(index_path):
            try:
                with open(index_path, "r") as file:
                    content = file.read()
                    self.load_from_serialized_str(content)
            except Exception:
                print(f"No index created, pass")
                pass

    def insert(self, keys_set, value):
        # Use frozenset to make the set of strings immutable (hashable)
        self.data[frozenset(sorted(keys_set))] = value

    def insert_without_overwriting(self, keys_set, value):
        """
        when there is already a k-v pair in data,
        try to put the existing value and the new value into a set,
        and if value is a set, please don't use this function.
        """
        if frozenset(keys_set) not in self.data:
            self.data[frozenset(keys_set)] = value
        existing_value = self.data[frozenset(keys_set)]
        if type(value) == set:
            raise TypeError(
                "Don't insert (_,set) var function insert_without_overwriting, please custom the insertion function"
            )
        if type(existing_value) == type(value):
            self.data[frozenset(keys_set)] = {
                self.data[frozenset(keys_set)], value}
        elif hasattr(existing_value, "add") and callable(getattr(object, "add")):
            self.data[frozenset(keys_set)].add(value)

    def _find_best_match_noncompulsive_subset(self, target_set):
        min_difference = float("inf")
        best_key = None

        for key in self.data:
            difference_set = target_set - key
            if len(difference_set) < min_difference:
                min_difference = len(difference_set)
                best_key = key
        if best_key:
            return best_key, target_set - best_key
        else:
            return best_key, target_set

    def _find_best_match_subset(self, target_set, original_set):
        min_difference = float("inf")
        best_key = None
        difference_set = {}
        for key in self.data:
            difference_set = target_set - key

            # Ensure key is subset of original_set and has an intersection with target_set
            if key.issubset(original_set) and len(difference_set) < len(target_set):
                if len(difference_set) < min_difference:
                    min_difference = len(difference_set)
                    best_key = key
        return best_key, difference_set

    def get_values_noncompulsive_subset(self, target_set):
        """
        The key corresponding to the found value is not necessarily a subset of target set
        """
        result = []
        while True:
            best_key, diff_set = self._find_best_match_noncompulsive_subset(
                target_set)

            # If no more matching keys or no difference set exists, break out of the loop
            if not best_key or not diff_set or diff_set == target_set:
                if best_key:
                    result.append((diff_set, best_key, self.data[best_key]))
                else:
                    result.append((diff_set, None, None))
                break

            if best_key:
                result.append((diff_set, best_key, self.data[best_key]))
            else:
                result.append((diff_set, None, None))

            target_set = diff_set

        return result

    def get_values_subset(self, target_set) -> list:
        '''
        if target_set is an array, it will be convert into a set
        returns: [(diff_set, best_key, the model info corresponding to the best_key )]
        '''
        if isinstance(target_set, list):
            target_set = set(target_set)
        result = []
        original_set = frozenset(target_set)

        while True:
            best_key, diff_set = self._find_best_match_subset(
                target_set, original_set)
            if best_key:
                result.append((diff_set, best_key, self.data[best_key]))
            elif diff_set.__len__() > 0:
                result.append((diff_set, None, None))

            target_set = diff_set
            # If no more matching keys or no difference set exists, break out of the loop
            if not best_key or not diff_set or diff_set == target_set:
                break

        return result

    def serializable_data(self) -> dict:
        return {'|'.join(sorted(key)): value for key, value in self.data.items()}

    def load_from_serialized_str(self, data_str: str):
        if data_str == '':
            return
        raw_data = json.loads(data_str.replace("\'", "\""))
        for key, value in raw_data.items():
            # Split the string back into a set
            elements = key.split('|')
            if elements[0] == '':
                elements = elements[1:]
            key_set = set(elements)
            self.insert(key_set, value)
        # return cm
