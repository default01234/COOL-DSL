class DataLoaderConfig:
    def __init__(self,batch_size = 16, train_eval_split = 0.8, pred = 'position'):
        self.train_eval_split = train_eval_split  # Proportion of data used for training (the rest for evaluation)
        # Include additional configuration parameters as needed
        self.batch_size = batch_size
        # Other parameters...
        self.pred = pred

# Define a Dataset class
from torch_geometric.data import DataLoader, Dataset
import networkx as nx
import torch_geometric.utils as utils
from extractTrainingDataFromJsonFile import *


import torch
device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')        
# device = torch.device('cpu')        
from torch_geometric.data import Data
from torch_geometric.utils.convert import from_networkx
import torch.nn.functional as F
import networkx as nx
import torch
import os
import pickle
import random

class ASTGraphDataset(Dataset):
    def __init__(self, ast_graphs:list[ASTNetworkXGraph], knowledge_domain_to_index,  type_to_index, identifier_to_index,string_to_index,operator_to_index, root=None, transform=None, pre_transform=None, pred = 'position'):
        self.ast_graphs = ast_graphs
        # Retrieve the mappings
        self.knowledge_domain_to_index = knowledge_domain_to_index
        self.type_to_index = type_to_index
        self.identifier_to_index = identifier_to_index
        self.string_to_index = string_to_index
        self.operator_to_index = operator_to_index
        self.pred = pred
        super(ASTGraphDataset, self).__init__(root, transform, pre_transform)

    def __len__(self):
        return len(self.ast_graphs)
    
    def len(self):
        return len(self.ast_graphs)
    
    def convert_to_one_hot(self, array, num_classes=3):
        # Maps -1 to 0, 0 to 1, and 1 to 2
        array_mapped = array + 1
        # Applies one-hot encoding
        one_hot = F.one_hot(array_mapped.to(torch.int64), num_classes=num_classes)
        # Reshape as a flattened vector if necessary
        return one_hot.view(-1, array.size(1) * num_classes)

    def get(self, idx):
        ast_networkx_graph:ASTNetworkXGraph = self.ast_graphs[idx]
        data = from_networkx(ast_networkx_graph.graph)
        


        # Dynamically add the attributes during Data object creation
        grounded = []
        boundtfdomain_indices = []
        root = []
        changeable = []
        type_indices = []
        identifier_indices = []
        string_indices = []
        number = []
        operator_indices = []
        step = []
        applied = []
        # jumps = [] 
        branch = []
        advance = []


        for _, node_data in ast_networkx_graph.graph.nodes(data=True):
            boundtfdomain_idx = self.knowledge_domain_to_index.get(node_data.get('boundtfdomain'), 0)
            node_root = node_data.get('root',0)
            node_changeable = node_data.get('changeable',0)
            type_idx = self.type_to_index.get(node_data.get('type'), 0)
            identifier_idx = self.identifier_to_index.get(node_data.get('identifier'), 0)
            string_idx = self.string_to_index.get(node_data.get('string'), 0)
            node_number = node_data.get('number',0)
            operator_idx = self.operator_to_index.get(node_data.get('operator'), 0)
            node_step = node_data.get('step',0)
            node_applied = node_data.get('applied',0)
            # node_jumps = node_data.get('jumps',0)
            node_branch = node_data.get('branch',0)
            node_advance = node_data.get('advance',0)
            node_grounded = node_data.get('grounded',0)

            grounded.append(node_grounded)
            boundtfdomain_indices.append(boundtfdomain_idx)
            root.append(node_root)
            changeable.append(node_changeable)
            type_indices.append(type_idx)
            identifier_indices.append(identifier_idx)
            string_indices.append(string_idx)
            number.append(node_number)
            operator_indices.append(operator_idx)
            step.append(node_step)
            applied.append(node_applied)
            # jumps.append(node_jumps)
            branch.append(node_branch)
            advance.append(node_advance)
            


        # Convert lists to tensors and attach to data object
        data.grounded = F.one_hot(torch.tensor(grounded,dtype=torch.long, device= device).to(torch.int64), num_classes=2)
        data.boundtfdomain_indices = torch.tensor(boundtfdomain_indices, dtype=torch.long,device= device)
        data.root = F.one_hot(torch.tensor(root,dtype=torch.long, device= device).to(torch.int64), num_classes=2)
        data.changeable = F.one_hot(torch.tensor(changeable, dtype=torch.long,device= device).to(torch.int64), num_classes=2)
        data.type_indices = torch.tensor(type_indices, dtype=torch.long,device= device)
        data.identifier_indices = torch.tensor(identifier_indices, dtype=torch.long,device= device)
        data.string_indices = torch.tensor(string_indices, dtype=torch.long,device= device)
        data.number= torch.tensor(number, dtype=torch.long,device= device)
        data.operator_indices = torch.tensor(operator_indices, dtype=torch.long,device= device)
        data.step= torch.tensor(step, dtype=torch.long,device= device).to(torch.int64)
        data.applied= torch.tensor(applied, dtype=torch.long,device= device)
        # One-hot encode `jumps` and `branch`
        # data.jumps = self.convert_to_one_hot(torch.tensor(jumps, dtype=torch.long, device=device))  # [node_number, max_ast_depth * 3]
        data.branch = self.convert_to_one_hot(torch.tensor(branch, dtype=torch.long, device=device).view(-1,1))  # [node_number, 3]
        # data.advance= F.one_hot(torch.tensor(advance, dtype=torch.long,device= device).to(torch.int64), num_classes=10) #at most 10 steps
        data.advance= torch.tensor(advance, dtype=torch.long,device= device).to(torch.int64)

        data.label_indomain = F.one_hot(torch.tensor(ast_networkx_graph.indomain, dtype=torch.long,device= device).to(torch.int64), num_classes=2)
        data.label_succeed = F.one_hot(torch.tensor(ast_networkx_graph.succeed, dtype=torch.long,device= device).to(torch.int64), num_classes=2)
        data.label_jumps = self.convert_to_one_hot(torch.tensor(ast_networkx_graph.applied_jumps, dtype=torch.long, device=device).view(-1,max_ast_depth))  # [graph_number, max_ast_depth * 3]
        data.label_advance = torch.tensor(ast_networkx_graph.advance, dtype=torch.long,device= device).to(torch.int64)
        data.label_rpsign = F.one_hot(torch.tensor(ast_networkx_graph.rpsign, dtype=torch.long,device= device).to(torch.int64), num_classes=2) # 0 for lt 0, 1 for gt 0
        data.label_rpval = torch.tensor(ast_networkx_graph.rpval, dtype=torch.float,device= device)
        data.label_rule = F.one_hot(torch.tensor(ast_networkx_graph.rule, dtype=torch.long,device= device).to(torch.int64), num_classes=2) # 0 for fact tf, 1 for rule tf

 

        
        data = data.to(device)
        return data

# Convert the list of ASTNetworkXGraph to DataLoader

class ASTGraphDataLoader:
    def __init__(self, ast_graphs: list[ASTNetworkXGraph], config: DataLoaderConfig, index_file_path: str = 'index_file.pkl'):
        self.ast_graphs = ast_graphs
        self.config = config
        self.index_file_path = index_file_path

        # Check if index file exists to load existing mappings or create new ones
        if os.path.exists(index_file_path):
            with open(index_file_path, 'rb') as f:
                index_mappings = pickle.load(f)
            self.knowledge_domain_to_index, self.type_to_index, self.identifier_to_index, \
                self.string_to_index, self.operator_to_index = index_mappings
            self.update_index()
            
        else:
            index_mappings = self.create_index()
        # Save the mappings to a file using pickle
        with open(index_file_path, 'wb') as f:
            pickle.dump(index_mappings, f)

        self.knowledge_domain_to_index, self.type_to_index, self.identifier_to_index, \
            self.string_to_index, self.operator_to_index = index_mappings
       
        self.split_dataset()

    def update_graphs(self,ast_graphs: list[ASTNetworkXGraph],config: DataLoaderConfig,index_file_path: str = 'index_file.pkl'):
        self.ast_graphs = ast_graphs
        self.config = config
        self.index_file_path = index_file_path

        new_element = self.update_index()
        if(new_element == True):
            with open(self.index_file_path, 'wb') as f:
                pickle.dump((self.knowledge_domain_to_index, self.type_to_index, self.identifier_to_index, \
            self.string_to_index, self.operator_to_index), f)
        self.split_dataset()  


    def create_index(self):
        # Initialize dictionaries with None mapped to 0 from the start
        knowledge_domain_to_index, type_to_index, identifier_to_index, string_to_index, operator_to_index = [{None: 0} for _ in range(5)]

        # Populate the dictionaries by iterating over ast_graphs
        for ast_graph in self.ast_graphs:
            for _, node_data in ast_graph.graph.nodes(data=True):
                # For each attribute, check if it's not None and not already in the dictionary
                if node_data['boundtfdomain'] is not None and node_data['boundtfdomain'] not in knowledge_domain_to_index:
                    knowledge_domain_to_index[node_data['boundtfdomain']] = len(knowledge_domain_to_index)+1

                if node_data['type'] is not None and node_data['type'] not in type_to_index:
                    type_to_index[node_data['type']] = len(type_to_index)+1
                
                if node_data['identifier'] is not None and node_data['identifier'] not in identifier_to_index:
                    identifier_to_index[node_data['identifier']] = len(identifier_to_index)+1
                
                if node_data['string'] is not None and node_data['string'] not in string_to_index:
                    string_to_index[node_data['string']] = len(string_to_index)+1
                
                if node_data['operator'] is not None and node_data['operator'] not in operator_to_index:
                    operator_to_index[node_data['operator']] = len(operator_to_index)+1

        return knowledge_domain_to_index, type_to_index, identifier_to_index, string_to_index, operator_to_index
    
    def update_index(self):
        # Populate the dictionaries by iterating over ast_graphs
        new_element = False
        for ast_graph in self.ast_graphs:
            for _, node_data in ast_graph.graph.nodes(data=True):
                # For each attribute, check if it's not None and not already in the dictionary
                if node_data['boundtfdomain'] is not None and node_data['boundtfdomain'] not in self.knowledge_domain_to_index:
                    self.knowledge_domain_to_index[node_data['boundtfdomain']] = len(self.knowledge_domain_to_index)+1
                    new_element = True

                if node_data['type'] is not None and node_data['type'] not in self.type_to_index:
                    self.type_to_index[node_data['type']] = len(self.type_to_index)+1
                    new_element = True
                
                if node_data['identifier'] is not None and node_data['identifier'] not in self.identifier_to_index:
                    self.identifier_to_index[node_data['identifier']] = len(self.identifier_to_index)+1
                    new_element = True
                
                if node_data['string'] is not None and node_data['string'] not in self.string_to_index:
                    self.string_to_index[node_data['string']] = len(self.string_to_index)+1
                    new_element = True
                
                if node_data['operator'] is not None and node_data['operator'] not in self.operator_to_index:
                    self.operator_to_index[node_data['operator']] = len(self.operator_to_index)+1
                    new_element = True
        return new_element
    def split_dataset(self):
        # Split the AST graphs based on the provided train/evaluation rate
        # Shuffle the dataset before splitting
        random.shuffle(self.ast_graphs)
        
        # Split the AST graphs based on the provided train/evaluation rate
        split_index = int(len(self.ast_graphs) * self.config.train_eval_split)
        train_graphs = self.ast_graphs[:split_index]
        eval_graphs = self.ast_graphs[split_index:]
        
        # Convert list of graphs to PyTorch Geometric Dataset instances
        self.train_dataset = ASTGraphDataset(train_graphs, self.knowledge_domain_to_index, self.type_to_index,self.identifier_to_index,self.string_to_index,self.operator_to_index, root='',pred=self.config.pred)
        self.eval_dataset = ASTGraphDataset(eval_graphs, self.knowledge_domain_to_index, self.type_to_index,self.identifier_to_index,self.string_to_index,self.operator_to_index, root='',pred=self.config.pred)
        return self.train_dataset, self.eval_dataset

    def get_data_loaders(self):
        # Create the DataLoader instances for the train and eval datasets
        train_loader = DataLoader(self.train_dataset, batch_size=self.config.batch_size, shuffle=True) if self.train_dataset.len()>0 else None
        eval_loader = DataLoader(self.eval_dataset, batch_size=self.config.batch_size, shuffle=False) if self.eval_dataset.len()>0 else None
        return train_loader, eval_loader
    

if __name__ == "__main__":
    # Instantiate DataLoaderConfig
    data_loader_config = DataLoaderConfig()

    example_graphs = initialize_from_file(filename="1__quadratic.json", pred= data_loader_config.pred) 
    # Assuming you've correctly defined your ASTGraphDataLoader with index file loading/saving:
    dataloader = ASTGraphDataLoader(example_graphs, data_loader_config, 'index_file.pkl')
    train_loader, eval_loader = dataloader.get_data_loaders()
    # Test the __len__ method
    assert (len(train_loader)+ len(eval_loader))* data_loader_config.batch_size >= len(example_graphs), "Length mismatch in ASTGraphDataset."

    # # Test the get method by retrieving a Data object from the dataset
    # data_object = dataset.get(0)
    # assert isinstance(data_object, Data), "The get method should return a Data object."

    print("Tests completed successfully.")

    