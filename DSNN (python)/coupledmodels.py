class ModelConfig:
    def __init__(
            self, embedding_sizes= {
            'boundtfdomain':50,  # Assuming 50 unique knowledge domains
            'type': 100,      # Assuming 100 unique types
            'identifier': 1000,  # Assuming 1000 unique identifiers
            'string': 1000,    # Assuming 1000 unique strings
            'operator': 50    # Assuming 50 unique operators
        },\
            # num_gat_features = 17,\
            gat_heads = 4,\
            hidden_gat_features = 64,\
            hidden_lstm_features= 64,\
            num_lstm_layer = 4,\
            hidden_linear_feature = 64,\
            # num_classes= 8, #output
            dropout_rate = 0.1,
            epochs = 200,
            batch_size = 16,
            learning_rate=0.001, 
            step_size=30, 
            gamma=0.1,
            pred = 'position',# prediction task
            showfig = True):
        self.dropout_rate = dropout_rate
        self.embedding_sizes = embedding_sizes
        self.indomain_output = 2
        self.succeed_output = 2
        self.jumps_output = max_ast_depth*3
        self.advance_output = 1
        self.rpsign_output = 2
        self.rpval_output = 1
        self.rule_output = 2

        self.boundtfdomain_embedding_dim = 1
        self.type_embedding_dim = 2
        self.identifier_embedding_dim = 4
        self.string_embedding_dim = 4
        self.operator_embedding_dim = 2

        if(pred == 'position'):
            self.num_gat_features = 2+self.boundtfdomain_embedding_dim+ 2 + 2+self.type_embedding_dim+self.identifier_embedding_dim+self.string_embedding_dim+ self.operator_embedding_dim+1 +1 +3 #ground 2/ tfdomain embedding/ root 2/ changeable 2/ type embedding / identifier embedding / string embedding / number 1/ operator /step 1/ branch 3 
            self.num_classes = 2+ 2 + max_ast_depth*3+ 1 + 2 + 1 + 2 #output size, 2 for indomain, 2 for succeed bit, jump output size, 1 for "advance" bit, 2 for "rpsign", 1 for "rpval", 2 for is_rule_fun 
        elif(pred == 'advance'):
            self.num_gat_features = 2+self.boundtfdomain_embedding_dim+ 2+2+self.type_embedding_dim+self.identifier_embedding_dim+self.string_embedding_dim+ self.operator_embedding_dim+1 +1 +3 + 1 # input size, 1 for "applied" bit
            self.num_classes = 2+ 2 + max_ast_depth*3 + 1 + 2 + 1+ 2 #output size, 2 for indomain, 2 for succeed bit, jump output size, 1 for "advance" bit, 2 for "rpsign", 1 for "rpval", 2 for is_rule_fun 
        elif(pred == 'reward'):
            self.num_gat_features = 2+self.boundtfdomain_embedding_dim+ 2+2+self.type_embedding_dim+self.identifier_embedding_dim+self.string_embedding_dim+ self.operator_embedding_dim+1 +1 +3 + 1 + 1# input size, 1 for "applied" bit, 1 for "advance" bit
            self.num_classes = 2+ 2 + max_ast_depth*3 + 1 + 2 + 1+ 2 #output size,  2 for indomain, 2 for succeed bit, jump output size,1 for "advance" bit, 2 for "rpsign", 1 for "rpval", 2 for is_rule_fun 
        elif(pred == 'solo'):
            self.num_gat_features = 2+self.boundtfdomain_embedding_dim+ 2+2+self.type_embedding_dim+self.identifier_embedding_dim+self.string_embedding_dim+ self.operator_embedding_dim+1 +1 +3 # input size
            self.num_classes = 2+ 2 + max_ast_depth*3 + 1 + 2 + 1 + 2#output size,  2 for indomain, 2 for succeed bit, jump output size, 1 for "advance" bit, 2 for "rpsign", 1 for "rpval", 2 for is_rule_fun 
        else:
            raise ValueError(f"Config Err")

        

        # GAT Parameters
        
        self.gat_heads = gat_heads
        self.hidden_gat_features = hidden_gat_features

        # LSTM Parameters
        self.hidden_lstm_features = hidden_lstm_features
        self.num_lstm_layer = num_lstm_layer

        #Linear Parameters
        self.hidden_linear_features = hidden_linear_feature
        self.learning_rate = learning_rate
        self.step_size = step_size
        self.gamma = gamma
        self.epochs=epochs
        self.batch_size = batch_size

        # task
        self.pred = pred

        # Visualization
        self.showfig = showfig
        
        
import torch
import torch.nn as nn
import torch.nn.functional as F
from torch_geometric.nn import GATConv,global_mean_pool
from torch.nn.utils.rnn import pack_padded_sequence, pad_packed_sequence, pack_sequence
from torch_geometric.data import Data
from torch_geometric.data import Batch
import torch.optim as optim
from torch.optim.lr_scheduler import StepLR
from torch.optim.lr_scheduler import ReduceLROnPlateau, CosineAnnealingLR

from extractTrainingDataFromJsonFile import *
import os
os.environ['KMP_DUPLICATE_LIB_OK']='TRUE'


class GAT_LSTM_Model(nn.Module):
    def __init__(self, config: ModelConfig):
        super(GAT_LSTM_Model, self).__init__()
        self.config = config
        self.optimizer = None
        self.scheduler = None
        # Embedding layers for string attributes
        self.boundtfdomain_embedding = nn.Embedding(num_embeddings=config.embedding_sizes['boundtfdomain'], embedding_dim=config.boundtfdomain_embedding_dim).to(device)
        self.type_embedding = nn.Embedding(num_embeddings=config.embedding_sizes['type'], embedding_dim=config.type_embedding_dim).to(device)
        self.identifier_embedding = nn.Embedding(num_embeddings=config.embedding_sizes['identifier'], embedding_dim=config.identifier_embedding_dim).to(device)
        self.string_embedding = nn.Embedding(num_embeddings=config.embedding_sizes['string'], embedding_dim=config.string_embedding_dim).to(device)
        self.operator_embedding = nn.Embedding(num_embeddings=config.embedding_sizes['operator'], embedding_dim=config.operator_embedding_dim).to(device)
        
        # Define GAT layers
        self.gat1 = GATConv(in_channels=config.num_gat_features, out_channels = config.num_gat_features, heads=self.config.gat_heads, concat=True, dropout=0).to(device)
        self.gat2 = GATConv(in_channels = config.num_gat_features * self.config.gat_heads, out_channels = config.hidden_lstm_features, heads=self.config.gat_heads, concat=False, dropout=config.dropout_rate).to(device)
        # self.gat3 = GATConv(in_channels = config.hidden_gat_features * self.config.gat_heads, out_channels = config.hidden_lstm_features, heads=self.config.gat_heads, concat=False, dropout=config.dropout_rate).to(device)

        # Define LSTM layers
        self.lstm1 = nn.LSTM(input_size=config.hidden_lstm_features, hidden_size=config.hidden_lstm_features, batch_first=True,bidirectional=True,num_layers=self.config.num_lstm_layer, dropout=config.dropout_rate).to(device)
        # self.lstm2 = nn.LSTM(input_size=config.hidden_lstm_features*2, hidden_size=config.hidden_lstm_features, batch_first=True,bidirectional=True, dropout=0).to(device)
        
        # Define Linear layers
        self.fc1 = nn.Linear(config.hidden_lstm_features*2, config.hidden_linear_features).to(device)
        self.bn1 = nn.BatchNorm1d(config.hidden_linear_features).to(device)
        # self.fc2 = nn.Linear(config.hidden_linear_features, config.hidden_linear_features).to(device)
        # self.bn2 = nn.BatchNorm1d(config.hidden_linear_features).to(device)

        # Task-specific output layers
        self.fc_indomain = nn.Linear(config.hidden_linear_features, config.indomain_output).to(device)  # Binary classification
        self.fc_succeed = nn.Linear(config.hidden_linear_features, config.succeed_output).to(device)  # Binary classification
        self.fc_jumps = nn.Linear(config.hidden_linear_features, config.jumps_output).to(device)  # Multi-class classification
        self.fc_advance = nn.Linear(config.hidden_linear_features, config.advance_output).to(device)  # Regression
        self.fc_rpsign = nn.Linear(config.hidden_linear_features, config.rpsign_output).to(device)  # Binary classification
        self.fc_rpval = nn.Linear(config.hidden_linear_features, config.rpval_output).to(device)  # Regression
        self.fc_rule = nn.Linear(config.hidden_linear_features, config.rule_output).to(device)  # Binary classification

        # Learnable parameters for task weighting
        self.log_var_indomain = nn.Parameter(torch.zeros(()))
        self.log_var_succeed = nn.Parameter(torch.zeros(()))
        self.log_var_jumps = nn.Parameter(torch.zeros(()))
        self.log_var_advance = nn.Parameter(torch.zeros(()))
        self.log_var_rpsign = nn.Parameter(torch.zeros(()))
        self.log_var_rpval = nn.Parameter(torch.zeros(()))
        self.log_var_rule = nn.Parameter(torch.zeros(()))

        self.relu = nn.ReLU().to(device)

        # Dropout layer
        self.dropout = nn.Dropout(p=config.dropout_rate)

    
    def forward(self, data:Data):
        x, edge_index = data.x, data.edge_index.to(device) 
        batch_size = data.num_graphs
        batch = data.batch #[node number,1]
        num_nodes = data.num_nodes
        wrap_offset = 700
        # Example attribute indices from data object, these need to be provided correctly
        
        grounded = data.grounded # size = [node number]
        boundtfdomain_indices = data.boundtfdomain_indices # size = [node number]
        root = data.root # size = [node number, 2]
        changeable = data.changeable #size = [node number]
        type_indices = data.type_indices #size = [node number]
        identifier_indices = (data.identifier_indices - wrap_offset) % (self.identifier_embedding.num_embeddings - wrap_offset) + wrap_offset
        identifier_indices = torch.where(data.identifier_indices < wrap_offset, data.identifier_indices, identifier_indices)

        string_indices = (data.string_indices - wrap_offset) % (self.string_embedding.num_embeddings - wrap_offset) + wrap_offset
        string_indices = torch.where(data.string_indices < wrap_offset, data.string_indices, string_indices)#size = [node number]
        
        number = data.number #size = [node number]
        operator_indices = data.operator_indices #size = [node number]
        step = data.step #size = [node number]
        applied = data.applied #size = [node number]
        # jumps = data.jumps #size = [node number,max_ast_depth]
        branch = data.branch #size = [node number, 1]
        advance = data.advance #size = [node number]

        # Run categorical attributes through embedding layers
        type = self.type_embedding(type_indices)
        boundtfdomain = self.boundtfdomain_embedding(boundtfdomain_indices)
        identifier = self.identifier_embedding(identifier_indices)
        string = self.string_embedding(string_indices)
        operator = self.operator_embedding(operator_indices)

        #reshape into x
        grounded = grounded.view(-1,2) #size = [node number,1]
        boundtfdomain = boundtfdomain.view(-1,self.config.boundtfdomain_embedding_dim )  #size = [node number,1]
        root = root.view(-1,2) #size = [node number,2]
        changeable = changeable.view(-1,2) #size = [node number,2]
        type = type.view(-1,self.config.type_embedding_dim) #size = [node number,1]
        identifier = identifier.view(-1,self.config.identifier_embedding_dim) #size = [node number,1]
        string = string.view(-1,self.config.string_embedding_dim) #size = [node number,1]
        number = number.view(-1,1) #size = [node number,1]
        operator = operator.view(-1,self.config.operator_embedding_dim) #size = [node number,1]
        step = step.view(-1,1) #size = [node number,1]
        applied = applied.view(-1,1) #size = [node number,1]
        # jumps = jumps.view(-1,max_ast_depth*3) #size = [node number,max_ast_depth*3], every three-bit group is a one hot, -1,0,1 -> 1,0,0; 0,1,0; 0,0,1
        branch = branch.view(-1,3) #size = [node number, 1*3], one hot, -1,0,1 -> 1,0,0; 0,1,0; 0,0,1
        advance = advance.view(-1,1) #size = [node number,1(step len)]

        # Concatenate embedded categorical attributes with numerical attributes
        if (self.config.pred == "position" or self.config.pred == "solo"):
            x = torch.cat(
                (
                    grounded,
                    boundtfdomain,
                    root,
                    changeable,
                    type,
                    identifier,
                    string,
                    number,
                    operator,
                    step,
                    # applied,
                    # jumps,
                    branch, # 3 bits
                    # advance,
                ), dim=1
            ) #[node number, feature number]

        elif self.config.pred == 'advance':
            x = torch.cat(
                (
                    grounded,
                    boundtfdomain,
                    root,
                    changeable,
                    type,
                    identifier,
                    string,
                    number,
                    operator,
                    step,
                    applied, #1 bit
                    # jumps,
                    branch, # 3 bits
                    # advance,
                ), dim=1
            ) #[node number, feature number]

        elif self.config.pred == 'reward':
            x = torch.cat(
                (
                    grounded,
                    boundtfdomain,
                    root,
                    changeable,
                    type,
                    identifier,
                    string,
                    number,
                    operator,
                    step,
                    applied, #1 bit
                    # jumps,
                    branch, # 3 bits
                    advance, # 1 bits
                ), dim=1
            ) #[node number, feature number]

        # Apply GAT layers
        gnn_output1 = (self.gat1(x, edge_index))
        gnn_output2 = (self.gat2(gnn_output1, edge_index))
        # gnn_output3 = (self.gat3(gnn_output2, edge_index))
        data.x = gnn_output2
        
        ##### following logic
        # rnn_output = []
        # for graph_id  in range(batch_size):
        #     graph_mask = (batch == graph_id)
        #     rnn_input1 = gnn_output3[graph_mask]
        #     rnn_output1, (hn, cn) = self.lstm1(rnn_input1)
        #     rnn_output2, (hn, cn) = self.lstm2(rnn_output1)
        #     rnn_output.append(rnn_output2[-1])
        # lin_input1 = torch.stack(rnn_output)
        # lin_output1 = self.fc1(lin_input1)
        # lin_output2 = self.fc2(lin_output1)

        # Parallel process
        rnn_inputs = [gnn_output2[batch == graph_id] for graph_id in range(batch_size)]
        lengths = torch.tensor([len(seq) for seq in rnn_inputs])
        padded_rnn_inputs = torch.nn.utils.rnn.pad_sequence(rnn_inputs, batch_first=True)
        packed_rnn_input = pack_padded_sequence(padded_rnn_inputs, lengths, batch_first=True, enforce_sorted=False)
        # Process sequences through the LSTM
        packed_rnn_output1, (hn1, cn1) = self.lstm1(packed_rnn_input)
        rnn_output1, _ = pad_packed_sequence(packed_rnn_output1, batch_first=True)

        # packed_output2, (hn2, cn2) = self.lstm2(pack_padded_sequence(rnn_output1, lengths, batch_first=True, enforce_sorted=False))
        # rnn_output2, _ = pad_packed_sequence(packed_output2, batch_first=True)
        rnn_output2 = rnn_output1
        lin_input1 = rnn_output2[torch.arange(rnn_output2.size(0)), lengths - 1]
        
        lin_output1 = self.fc1(lin_input1)
        # lin_output1 = self.bn1(lin_output1)
        # lin_output1 = self.relu(lin_output1)
        # lin_output1 = self.dropout(lin_output1)

        lin_output2 = lin_output1
        # lin_output2 = self.fc2(lin_output1)
        # lin_output2 = self.bn2(lin_output2)
        # lin_output2 = self.relu(lin_output2)


        # Task-specific outputs
        out_indomain = self.fc_indomain(lin_output2)
        out_succeed = self.fc_succeed(lin_output2)
        out_jumps = self.fc_jumps(lin_output2)
        out_advance = self.fc_advance(lin_output2)
        out_rpsign = self.fc_rpsign(lin_output2)
        out_rpval = self.fc_rpval(lin_output2)
        out_rule = self.fc_rule(lin_output2)


        combined_output = torch.cat((out_indomain, out_succeed, out_jumps, out_advance, out_rpsign, out_rpval, out_rule), dim=1)
        return combined_output


    def train_model(self, train_loader):
        super().train()  # Set the model in training mode
        epochs = self.config.epochs
        if self.optimizer == None:
            weight_decay = 0
            self.optimizer = optim.Adam(self.parameters(), lr=self.config.learning_rate,weight_decay=weight_decay)
        if self.scheduler == None:
            self.scheduler = ReduceLROnPlateau(self.optimizer, mode='min', factor=0.1, patience=10,threshold=1e-3, cooldown= 5, min_lr= 1e-5,verbose=True)

        # Check and adjust the learning rate after reloading 
        for param_group in self.optimizer.param_groups:
            if param_group['lr'] <= 1e-5:
                param_group['lr'] = 1e-4  # Adjust the learning rate

        #Initialization of losses and accuracies for visualization
        indomain_losses = []
        indomain_accuracies = []
        succeed_losses = []
        succeed_accuracies = []
        jump_losses = []
        jumps_accuracies = []
        advance_losses = []
        advance_accuracies = []
        rpsign_losses = []
        rpsign_accuracies = []
        rpval_losses = []
        rpval_accuracies = []
        rule_losses = []
        rule_accuracies = []
        

        criterionBCE = nn.BCEWithLogitsLoss()
        criterionCE = nn.CrossEntropyLoss()
        criterionHuber = nn.HuberLoss(delta=1.0)

        best_early_stopping_loss = float('inf')
        early_stopping_patience = 20
        early_stopping_epochs = 0

        for epoch in range(epochs):
            # current_lr = self.optimizer.param_groups[0]['lr']
            # current_dropout = self.config.dropout_rate
            # if current_lr >= 0.001:
            #     current_dropout =  self.config.dropout_rate
            # elif current_lr >= 0.0001:
            #     current_dropout =  self.config.dropout_rate/2
            # else:
            #     current_dropout =  0.0
            # # self.gat1.dropout = current_dropout
            # self.gat2.dropout = current_dropout
            # self.gat3.dropout = current_dropout
            # self.lstm1.dropout = current_dropout if self.lstm1.num_layers > 1 else 0
            # self.lstm2.dropout = current_dropout if self.lstm2.num_layers > 1 else 0
            # self.dropout.p = current_dropout

            early_stopping_loss = 0
            epoch_indomain_loss = 0
            correct_indomain = 0
            total_indomain = 0
            epoch_succeed_loss = 0
            correct_succeed = 0
            total_succeed = 0
            epoch_jumps_loss = 0
            correct_jumps = 0
            total_jumps = 0
            epoch_advance_loss = 0
            correct_advances = 0
            epoch_rpsign_loss = 0
            correct_rpsigns = 0
            epoch_rpval_loss = 0
            correct_rpvals = 0
            epoch_rule_loss = 0
            correct_rules = 0
            

            for batch in train_loader:
                self.optimizer.zero_grad()
                out = self.forward(batch) # assuming [graph num, output size * 3] 
                
                y = None
                
                y_succeed = batch.label_succeed.type(torch.float).view(-1,2)
                y_indomain = batch.label_indomain.type(torch.float).view(-1,2)
                y_jumps = batch.label_jumps.type(torch.float) # assuming shape is [graph num, output size * 3]
                y_advance = batch.label_advance.type(torch.float)
                y_rpsign = batch.label_rpsign.type(torch.float).view(-1,2)
                y_rpval = batch.label_rpval.type(torch.float)
                y_rule = batch.label_rule.type(torch.float).view(-1,2)

                out_indomain = out[:,:2].view(-1,2)
                out_succeed = out[:,2:2+2].view(-1,2)
                out_jumps = out[:,2+2: 2+2 + max_ast_depth*3]
                out_advance = out[:,2+2 + max_ast_depth*3]
                out_rpsign = out[:,2 +2+ max_ast_depth*3 + 1:2+2 + max_ast_depth*3 + 1 +2].view(-1,2)
                out_rpval = out[:,2+2 + max_ast_depth*3 + 1 +2]
                out_rule = out[:,2 +2+ max_ast_depth*3 + 1 +2 + 1:2+2 + max_ast_depth*3 + 1 +2 +1 +2].view(-1,2)

                indomain_loss = criterionBCE(out_indomain,y_indomain)

                indomain_succeed_jumps_advance_rpsign_rpval_rule_loss = 0
                
                # Masks for different scenarios
                mask_outdomain = (y_indomain[:, 0] == 1)  # y_indomain is (1,0) indicating failure
                mask_indomain = (y_indomain[:, 1] == 1)  # y_indomain is (0,1) indicating success

                mask_failed = ((y_indomain[:, 0] == 1) | (y_succeed[:, 0] == 1))  # y_succeed is (1,0) indicating failure
                mask_succeed = ((y_indomain[:, 1] == 1) & (y_succeed[:, 1] == 1))  # y_succeed is (0,1) indicating success
                
                succeed_loss = torch.tensor(0.0, requires_grad=True, device=device)
                jumps_loss = torch.tensor(0.0, requires_grad=True, device=device)
                advance_loss = torch.tensor(0.0, requires_grad=True, device=device)
                rpsign_loss = torch.tensor(0.0, requires_grad=True, device=device)
                rpval_loss = torch.tensor(0.0, requires_grad=True, device=device)
                rule_loss = torch.tensor(0.0, requires_grad=True, device=device)
                if mask_indomain.any():
                    succeed_loss = criterionBCE(out_succeed[mask_indomain], y_succeed[mask_indomain])

                    if mask_succeed.any():
                        # jumps_loss = criterionBCE(out_jumps, y_jumps)
                        # advance_loss = criterionHuber(out_advance, y_advance)
                        # rpsign_loss = criterionBCE(out_rpsign,y_rpsign)
                        # rpval_loss = criterionHuber(out_rpval, y_rpval)
                        jumps_loss = criterionBCE(out_jumps[mask_succeed].reshape(-1,3), y_jumps[mask_succeed].reshape(-1,3))
                        # jumps_loss = criterionBCE(out_jumps[mask_succeed], y_jumps[mask_succeed])
                        advance_loss = criterionHuber(out_advance[mask_succeed], y_advance[mask_succeed])
                        rpsign_loss = criterionBCE(out_rpsign[mask_succeed], y_rpsign[mask_succeed])
                        rpval_loss = criterionHuber(out_rpval[mask_succeed], y_rpval[mask_succeed])
                        rule_loss = criterionBCE(out_rule[mask_succeed], y_rule[mask_succeed])


                        # Precision (inverse of variance)
                        # precision_succeed = torch.exp(-self.log_var_succeed)
                        # precision_jumps = torch.exp(-self.log_var_jumps)
                        # precision_advance = torch.exp(-self.log_var_advance)
                        # precision_rpsign = torch.exp(-self.log_var_rpsign)
                        # precision_rpval = torch.exp(-self.log_var_rpval)
                        # # Adding a small regularization term to prevent log_var from becoming too negative
                        # regularization_term = 1e-6 * (
                        #     torch.exp(self.log_var_succeed) +
                        #     torch.exp(self.log_var_jumps) +
                        #     torch.exp(self.log_var_advance) +
                        #     torch.exp(self.log_var_rpsign) +
                        #     torch.exp(self.log_var_rpval)
                        # )

                        # indomain_succeed_jumps_advance_rpsign_rpval_rule_loss =  (
                        #     precision_succeed * succeed_loss + self.log_var_succeed +
                        #     precision_jumps * jumps_loss + self.log_var_jumps +
                        #     precision_advance * advance_loss + self.log_var_advance +
                        #     precision_rpsign * rpsign_loss + self.log_var_rpsign +
                        #     precision_rpval * rpval_loss + self.log_var_rpval + regularization_term)    
                        indomain_succeed_jumps_advance_rpsign_rpval_rule_loss = (indomain_loss+ 2*succeed_loss + 3*jumps_loss + advance_loss + rpsign_loss + rpval_loss + rule_loss)
                    else:
                        indomain_succeed_jumps_advance_rpsign_rpval_rule_loss = indomain_loss+ 2* succeed_loss
                else:
                    indomain_succeed_jumps_advance_rpsign_rpval_rule_loss = indomain_loss

                indomain_succeed_jumps_advance_rpsign_rpval_rule_loss.backward()
                self.optimizer.step()

                early_stopping_loss += (indomain_loss+ 2*succeed_loss + 3*jumps_loss + advance_loss + rpsign_loss + rpval_loss+ rule_loss)
                
                epoch_indomain_loss += indomain_loss.item()
                epoch_succeed_loss += succeed_loss.item()
                epoch_jumps_loss += jumps_loss.item()
                epoch_advance_loss += advance_loss.item()
                epoch_rpsign_loss += rpsign_loss.item()
                epoch_rpval_loss +=rpval_loss.item()
                epoch_rule_loss +=rule_loss.item()



                # Calculate accuracy (indomain)
                # First, get the predicted class by finding the max index in every 2 bits block of logits
                pred_indomain = (out_indomain.reshape(-1, 2).max(dim=1)[1]) # Adjust indices to match range [0, 1]
                label_indomain = (y_indomain.reshape(-1, 2).max(dim=1)[1])

                total_indomain += label_indomain.size(0)  # Total number of label vectors

                # Compare vectors
                for pred_vec, label_vec in zip(pred_indomain, label_indomain):
                    if torch.equal(pred_vec, label_vec):
                        correct_indomain += 1


                # Calculate accuracy (succeed)
                # First, get the predicted class by finding the max index in every 2 bits block of logits
                pred_succeed = (out_succeed[mask_indomain].reshape(-1, 2).max(dim=1)[1]) # Adjust indices to match range [0, 1]
                label_succeed = (y_succeed[mask_indomain].reshape(-1, 2).max(dim=1)[1])

                total_succeed += label_succeed.size(0)  # Total number of label vectors

                # Compare vectors
                for pred_vec, label_vec in zip(pred_succeed, label_succeed):
                    if torch.equal(pred_vec, label_vec):
                        correct_succeed += 1

                

                # Calculate accuracy (jumps)
                # First, get the predicted class by finding the max index in every 3 bits block of logits
                pred_jumps = (out_jumps[mask_succeed].reshape(-1, 3).max(dim=1)[1] - 1).view(-1, max_ast_depth)  # Adjust indices to match range [-1, 0, 1]
                label_jumps = (y_jumps[mask_succeed].reshape(-1, 3).max(dim=1)[1] - 1).view(-1, max_ast_depth)

                total_jumps += label_jumps.size(0)  # Total number of label vectors

                # Compare vectors
                for pred_vec, label_vec in zip(pred_jumps, label_jumps):
                    if torch.equal(pred_vec, label_vec):
                        correct_jumps += 1
                

                # Calculate accuracy (advance)
                pred_advance = (out_advance[mask_succeed])
                label_advance = (y_advance[mask_succeed])
                if pred_advance.size(0) != pred_jumps.size(0):
                    raise ValueError('Misaligned values')
                # Compare vectors
                for pred_vec, label_vec in zip(pred_advance, label_advance):
                    if torch.equal(torch.round(pred_vec), torch.round(label_vec)):
                        correct_advances += 1

                # Calculate accuracy (rpsign)
                # First, convert the 2 bits classification to 1 bit number
                pred_rpsign = (out_rpsign[mask_succeed].reshape(-1, 2).max(dim=1)[1])
                label_rpsign = (y_rpsign[mask_succeed].reshape(-1, 2).max(dim=1)[1])
                if pred_rpsign.size(0) != pred_jumps.size(0):
                    raise ValueError('Misaligned values')
                # Compare vectors
                for pred_vec, label_vec in zip(pred_rpsign, label_rpsign):
                    if torch.equal(pred_vec, label_vec):
                        correct_rpsigns += 1

                # Calculate accuracy (rpval)
                pred_rpval = out_rpval[mask_succeed]
                label_rpval = y_rpval[mask_succeed]
                if pred_rpval.size(0) != pred_jumps.size(0):
                    raise ValueError('Misaligned values')
                # Compare vectors
                for pred_vec, label_vec in zip(pred_rpval, label_rpval):
                    if (torch.abs(pred_vec - label_vec) <= 0.3 * torch.abs(label_vec)):
                        correct_rpvals += 1

                # Calculate accuracy (rule)
                # First, convert the 2 bits classification to 1 bit number
                pred_rule = (out_rule[mask_succeed].reshape(-1, 2).max(dim=1)[1])
                label_rule = (y_rule[mask_succeed].reshape(-1, 2).max(dim=1)[1])
                if pred_rule.size(0) != pred_jumps.size(0):
                    raise ValueError('Misaligned values')
                # Compare vectors
                for pred_vec, label_vec in zip(pred_rule, label_rule):
                    if torch.equal(pred_vec, label_vec):
                        correct_rules += 1

            for param_group in self.optimizer.param_groups:
                current_lr = param_group['lr']
                print(f"Epoch {epoch + 1}: Current Learning Rate = {current_lr}")

            # self.scheduler.eps = early_stopping_loss*0.05s
            self.scheduler.step(early_stopping_loss)

            # report
            if (self.config.pred in ['position', 'advance', 'reward', 'solo']):
                epoch_indomain_avg_loss = epoch_indomain_loss / len(train_loader)
                epoch_indomain_accuracy = 0 if total_indomain == 0 else correct_indomain / total_indomain
                epoch_succeed_avg_loss = epoch_succeed_loss / len(train_loader)
                epoch_succeed_accuracy = 0 if total_succeed == 0 else correct_succeed / total_succeed
                epoch_jumps_avg_loss = epoch_jumps_loss / len(train_loader)
                epoch_jumps_accuracy = 0 if total_jumps == 0 else correct_jumps / total_jumps
                epoch_advance_avg_loss = epoch_advance_loss / len(train_loader)
                epoch_advance_accuracy = 0 if total_jumps == 0 else correct_advances / total_jumps
                epoch_rpsign_avg_loss = epoch_rpsign_loss / len(train_loader)
                epoch_rpsign_accuracy = 0 if total_jumps == 0 else correct_rpsigns / total_jumps
                epoch_rpval_avg_loss = epoch_rpval_loss / len(train_loader)
                epoch_rpval_accuracy = 0 if total_jumps == 0 else correct_rpvals / total_jumps
                epoch_rule_avg_loss = epoch_rule_loss / len(train_loader)
                epoch_rule_accuracy = 0 if total_jumps == 0 else correct_rules / total_jumps

                indomain_losses.append(epoch_indomain_avg_loss)
                indomain_accuracies.append(epoch_indomain_accuracy)
                succeed_losses.append(epoch_succeed_avg_loss)
                succeed_accuracies.append(epoch_succeed_accuracy)
                jump_losses.append(epoch_jumps_avg_loss)
                jumps_accuracies.append(epoch_jumps_accuracy)
                advance_losses.append(epoch_advance_avg_loss)
                advance_accuracies.append(epoch_advance_accuracy)
                rpsign_losses.append(epoch_rpsign_avg_loss)
                rpsign_accuracies.append(epoch_rpsign_accuracy)
                rpval_losses.append(epoch_rpval_avg_loss)
                rpval_accuracies.append(epoch_rpval_accuracy)
                rule_losses.append(epoch_rule_avg_loss)
                rule_accuracies.append(epoch_rule_accuracy)
                print(f"Epoch {epoch + 1},\n"
                    f"  InDomain Loss: {epoch_indomain_avg_loss:.4f}, InDomain Accuracy: {epoch_indomain_accuracy:.4f}\n"
                    f"  Succeed Loss: {epoch_succeed_avg_loss:.4f}, Succeed Accuracy: {epoch_succeed_accuracy:.4f}\n"
                    f"  Jump Loss: {epoch_jumps_avg_loss:.4f}, Jump Accuracy: {epoch_jumps_accuracy:.4f}\n"
                    f"  Advance Loss: {epoch_advance_avg_loss:.4f}, Advance Accuracy: {epoch_advance_accuracy:.4f}\n"
                    f"  Rpsign Loss: {epoch_rpsign_avg_loss:.4f}, Rpsign Accuracy: {epoch_rpsign_accuracy:.4f}\n"
                    f"  Rpval Loss: {epoch_rpval_avg_loss:.4f}, Rpval Accuracy: {epoch_rpval_accuracy:.4f}\n"
                    f"  Rule Loss: {epoch_rule_avg_loss:.4f}, Rule Accuracy: {epoch_rule_accuracy:.4f}\n"
                    )

                
            if (early_stopping_loss < best_early_stopping_loss * 0.95) :
                # and (epoch_indomain_accuracy <=0.99 or epoch_succeed_accuracy <= 0.99 or epoch_jumps_accuracy <= 0.99 or epoch_advance_accuracy <=0.99 or epoch_rpsign_accuracy <=0.99 or epoch_rpval_accuracy <=0.99 or epoch_rule_accuracy <=0.99):
                
                best_early_stopping_loss = early_stopping_loss
                early_stopping_epochs = 0  # Reset counter
                # torch.save(model.state_dict(), 'best_model.pth')  # Save best model
                # print(f"Epoch {epoch}: New best loss {best_loss}")
            else:
                if not (epoch_indomain_accuracy <=0.99 or epoch_succeed_accuracy <= 0.99 or epoch_jumps_accuracy <= 0.99 or epoch_advance_accuracy <=0.99 or epoch_rpsign_accuracy <=0.99 or epoch_rpval_accuracy <=0.99 or epoch_rule_accuracy <=0.99):
                    early_stopping_epochs += early_stopping_patience//4
                else:
                    early_stopping_epochs += 1
                print(f"Epoch {epoch}: No improvement ({early_stopping_epochs}/{early_stopping_patience}:{early_stopping_loss}/{best_early_stopping_loss})")

            if early_stopping_epochs >= early_stopping_patience:
                print("Early stopping triggered")
                break
            # Avoid rapid decrease in learning rate when undertraining 
            if epoch_indomain_accuracy <=0.98 or epoch_succeed_accuracy <= 0.98 or epoch_jumps_accuracy <= 0.98 or epoch_advance_accuracy <=0.98 or epoch_rpsign_accuracy <=0.98 or epoch_rpval_accuracy <=0.98 or epoch_rule_accuracy <=0.98:
                for param_group in self.optimizer.param_groups :
                    if param_group['lr'] < 1e-4:
                        param_group['lr'] = 1e-4  # Adjust the learning rate
            
        if self.config.showfig:
            fig_row = None
            if (self.config.pred in ['position', 'advance', 'reward', 'solo']):
                fig_row = 7
            plt.figure(figsize=(12, 10))


            # Plotting InDomain Training Loss
            plt.subplot(fig_row, 2, 1)
            plt.plot(indomain_losses, label='InDomain Loss')
            plt.title('Indomain Training Loss')
            plt.xlabel('Epoch')
            plt.ylabel('Loss')
            plt.legend()

            # Plotting InDomain Training Accuracy
            plt.subplot(fig_row, 2, 2)
            plt.plot(succeed_accuracies, label='InDomain Accuracy', color='orange')
            plt.title('InDomain Training Accuracy')
            plt.xlabel('Epoch')
            plt.ylabel('Accuracy')
            plt.legend()

            # Plotting Succeed Training Loss
            plt.subplot(fig_row, 2, 3)
            plt.plot(succeed_losses, label='Training Loss')
            plt.title('Succeed Training Loss')
            plt.xlabel('Epoch')
            plt.ylabel('Loss')
            plt.legend()

            # Plotting Succeed Training Accuracy
            plt.subplot(fig_row, 2, 4)
            plt.plot(succeed_accuracies, label='Training Accuracy', color='orange')
            plt.title('Succeed Training Accuracy')
            plt.xlabel('Epoch')
            plt.ylabel('Accuracy')
            plt.legend()

            # Plotting Jump Training Loss
            plt.subplot(fig_row, 2, 5)
            plt.plot(jump_losses, label='Training Loss')
            plt.title('Jump Training Loss')
            plt.xlabel('Epoch')
            plt.ylabel('Loss')
            plt.legend()

            # Plotting Jump Training Accuracy
            plt.subplot(fig_row, 2, 6)
            plt.plot(jumps_accuracies, label='Training Accuracy', color='orange')
            plt.title('Jump Training Accuracy')
            plt.xlabel('Epoch')
            plt.ylabel('Accuracy')
            plt.legend()

            # Plotting Advance Training Loss
            plt.subplot(fig_row, 2, 7)
            plt.plot(advance_losses, label='Training Loss')
            plt.title('Advance Training Loss')
            plt.xlabel('Epoch')
            plt.ylabel('Loss')
            plt.legend()

            # Plotting Advance Training Accuracy
            plt.subplot(fig_row, 2, 8)
            plt.plot(advance_accuracies, label='Training Accuracy', color='orange')
            plt.title('Advance Training Accuracy')
            plt.xlabel('Epoch')
            plt.ylabel('Accuracy')
            plt.legend()

            # Plotting Rpsign Training Loss
            plt.subplot(fig_row, 2, 9)
            plt.plot(rpsign_losses, label='Training Loss')
            plt.title('Rpsign Training Loss')
            plt.xlabel('Epoch')
            plt.ylabel('Loss')
            plt.legend()

            # Plotting Rpsign Training Accuracy
            plt.subplot(fig_row, 2, 10)
            plt.plot(rpsign_accuracies, label='Training Accuracy', color='orange')
            plt.title('Rpsign Training Accuracy')
            plt.xlabel('Epoch')
            plt.ylabel('Accuracy')
            plt.legend()

            # Plotting Rpval Training Loss
            plt.subplot(fig_row, 2, 11)
            plt.plot(rpval_losses, label='Training Loss')
            plt.title('Rpval Training Loss')
            plt.xlabel('Epoch')
            plt.ylabel('Loss')
            plt.legend()

            # Plotting Rpval Training Accuracy
            plt.subplot(fig_row, 2, 12)
            plt.plot(rpval_accuracies, label='Training Accuracy', color='orange')
            plt.title('Rpval Training Accuracy')
            plt.xlabel('Epoch')
            plt.ylabel('Accuracy')
            plt.legend()

            # Plotting Rule Training Loss
            plt.subplot(fig_row, 2, 13)
            plt.plot(rule_losses, label='Training Loss')
            plt.title('Rule Training Loss')
            plt.xlabel('Epoch')
            plt.ylabel('Loss')
            plt.legend()

            # Plotting Rule Training Accuracy
            plt.subplot(fig_row, 2, 14)
            plt.plot(rule_accuracies, label='Training Accuracy', color='orange')
            plt.title('Rule Training Accuracy')
            plt.xlabel('Epoch')
            plt.ylabel('Accuracy')
            plt.legend()

            # Adjust layout to prevent overlap
            plt.tight_layout()

            # Display the plots
            plt.show()



    def eval_model(self, eval_loader):
        super().eval()  # Set the model in evaluation mode

        
        criterionBCE = nn.BCEWithLogitsLoss()
        criterionCE = nn.CrossEntropyLoss()
        criterionHuber = nn.HuberLoss(delta=1.0)

        
        # We don't need gradients for evaluation, so we disable them to save memory and computation
        with torch.no_grad():
            total_indomain_loss = 0
            correct_indomain = 0
            total_indomain = 0
            total_succeed_loss = 0
            correct_succeed = 0
            total_succeed = 0
            total_jumps = 0
            total_jumps_loss = 0
            correct_jumps = 0
            total_jumps = 0
            total_advance_loss = 0
            correct_advances = 0
            total_rpsign_loss = 0
            correct_rpsigns = 0
            total_rpval_loss = 0
            correct_rpvals = 0
            total_rule_loss = 0
            correct_rules = 0
            
            for batch in eval_loader:
                
                out = self.forward(batch) # assuming [graph num, output size * 3]

                y = None
                if self.config.pred in ['position','advance','reward','solo']:
                    y_indomain = batch.label_indomain.type(torch.float).view(-1,2)
                    y_succeed = batch.label_succeed.type(torch.float).view(-1,2)
                    y_jumps = batch.label_jumps.type(torch.float) # assuming shape is [graph num, output size * 3]
                    y_advance = batch.label_advance.type(torch.float)
                    y_rpsign = batch.label_rpsign.type(torch.float).view(-1,2)
                    y_rpval = batch.label_rpval.type(torch.float)
                    y_rule = batch.label_rule.type(torch.float).view(-1,2)


                    out_indomain = out[:,:2]
                    out_succeed = out[:,2:2+2]
                    out_jumps = out[:,2+2: 2 +2+ max_ast_depth*3]
                    out_advance = out[:,2+2 + max_ast_depth*3]
                    out_rpsign = out[:,2 +2+ max_ast_depth*3 + 1:2 +2+ max_ast_depth*3 + 1 +2].view(-1,2)
                    out_rpval = out[:,2 +2+ max_ast_depth*3 + 1 +2]
                    out_rule = out[:,2 +2+ max_ast_depth*3 + 1 +2+1:2 +2+ max_ast_depth*3 + 1 +2+1 +2].view(-1,2)
                    

                    indomain_loss = criterionBCE(out_indomain,y_indomain)

                    indomain_succeed_jumps_advance_rpsign_rpval_rule_loss = 0
                    
                    # Masks for different scenarios
                    mask_outdomain = (y_indomain[:, 0] == 1)  # y_indomain is (1,0) indicating failure
                    mask_indomain = (y_indomain[:, 1] == 1)  # y_indomain is (0,1) indicating success

                    mask_failed = ((y_indomain[:, 0] == 1) | (y_succeed[:, 0] == 1))  # y_succeed is (1,0) indicating failure
                    mask_succeed = ((y_indomain[:, 1] == 1) & (y_succeed[:, 1] == 1))  # y_succeed is (0,1) indicating success
                    
                    succeed_loss = torch.tensor(0.0, requires_grad=False, device=device)
                    jumps_loss = torch.tensor(0.0, requires_grad=False, device=device)
                    advance_loss = torch.tensor(0.0, requires_grad=False, device=device)
                    rpsign_loss = torch.tensor(0.0, requires_grad=False, device=device)
                    rpval_loss = torch.tensor(0.0, requires_grad=False, device=device)
                    rule_loss = torch.tensor(0.0, requires_grad=False, device=device)

                    if mask_indomain.any():
                        succeed_loss = criterionBCE(out_succeed[mask_indomain], y_succeed[mask_indomain])

                        if mask_succeed.any():
                            # jumps_loss = criterionBCE(out_jumps, y_jumps)
                            # advance_loss = criterionHuber(out_advance, y_advance)
                            # rpsign_loss = criterionBCE(out_rpsign,y_rpsign)
                            # rpval_loss = criterionHuber(out_rpval, y_rpval)
                            jumps_loss = criterionBCE(out_jumps[mask_succeed].reshape(-1,3), y_jumps[mask_succeed].reshape(-1,3))
                            # jumps_loss = criterionBCE(out_jumps[mask_succeed], y_jumps[mask_succeed])
                            advance_loss = criterionHuber(out_advance[mask_succeed], y_advance[mask_succeed])
                            rpsign_loss = criterionBCE(out_rpsign[mask_succeed], y_rpsign[mask_succeed])
                            rpval_loss = criterionHuber(out_rpval[mask_succeed], y_rpval[mask_succeed])
                            rule_loss = criterionBCE(out_rule[mask_succeed], y_rule[mask_succeed])


                            

                              
                            indomain_succeed_jumps_advance_rpsign_rpval_rule_loss = (indomain_loss+ 2*succeed_loss + 2*jumps_loss + advance_loss + rpsign_loss + rpval_loss + rule_loss)
                        else:
                            indomain_succeed_jumps_advance_rpsign_rpval_rule_loss = indomain_loss+ 2* succeed_loss
                    else:
                        indomain_succeed_jumps_advance_rpsign_rpval_rule_loss = indomain_loss

                                    

                    total_indomain_loss += indomain_loss.item()
                    total_succeed_loss += succeed_loss.item()
                    total_jumps_loss += jumps_loss.item()
                    total_advance_loss += advance_loss.item()
                    total_rpsign_loss += rpsign_loss.item()
                    total_rpval_loss += rpval_loss.item()                   
                    total_rule_loss += rule_loss.item()                   

                    # Calculate accuracy (indomain)
                    # First, get the predicted class by finding the max index in every 2 bits block of logits
                    pred_indomain = (out_indomain.reshape(-1, 2).max(dim=1)[1]) # Adjust indices to match range [0, 1]
                    label_indomain = (y_indomain.reshape(-1, 2).max(dim=1)[1])

                    total_indomain += label_indomain.size(0)  # Total number of label vectors

                    # Compare vectors
                    for pred_vec, label_vec in zip(pred_indomain, label_indomain):
                        if torch.equal(pred_vec, label_vec):
                            correct_indomain += 1
                            
                    # Calculate accuracy (succeed)
                    # First, get the predicted class by finding the max index in every 3 bits block of logits
                    pred_succeed = (out_succeed[mask_indomain].reshape(-1, 2).max(dim=1)[1]) # Adjust indices to match range [0, 1]
                    label_succeed = (y_succeed[mask_indomain].reshape(-1, 2).max(dim=1)[1])

                    total_succeed += label_succeed.size(0)  # Total number of label vectors

                    # Compare vectors
                    for pred_vec, label_vec in zip(pred_succeed, label_succeed):
                        if torch.equal(pred_vec, label_vec):
                            correct_succeed += 1
                            
                    # Calculate accuracy (jumps)
                    # First, get the predicted class by finding the max index in every 3 bits block of logits
                    pred_jumps = (out_jumps[mask_succeed].reshape(-1, 3).max(dim=1)[1] - 1).view(-1, max_ast_depth)  # Adjust indices to match range [-1, 0, 1]
                    label_jumps = (y_jumps[mask_succeed].reshape(-1, 3).max(dim=1)[1] - 1).view(-1, max_ast_depth)

                    total_jumps += label_jumps.size(0)  # Total number of label vectors

                    # Compare vectors
                    for pred_vec, label_vec in zip(pred_jumps, label_jumps):
                        if torch.equal(pred_vec, label_vec):
                            correct_jumps += 1

                    # Calculate accuracy (advance)
                    # First, convert the 10 bits classification to 1 bit number
                    pred_advance = (out_advance[mask_succeed])
                    label_advance = (y_advance[mask_succeed])
                    if pred_advance.size(0) != pred_jumps.size(0):
                        raise ValueError('Misaligned values')
                    # Compare vectors
                    for pred_vec, label_vec in zip(pred_advance, label_advance):
                        if torch.equal(torch.round(pred_vec), torch.round(label_vec)):
                            correct_advances += 1

                    # Calculate accuracy (rpsign)
                    # First, convert the 2 bits classification to 1 bit number
                    pred_rpsign = (out_rpsign[mask_succeed].reshape(-1, 2).max(dim=1)[1])
                    label_rpsign = (y_rpsign[mask_succeed].reshape(-1, 2).max(dim=1)[1])
                    if pred_rpsign.size(0) != pred_jumps.size(0):
                        raise ValueError('Misaligned values')
                    # Compare vectors
                    for pred_vec, label_vec in zip(pred_rpsign, label_rpsign):
                        if torch.equal(pred_vec, label_vec):
                            correct_rpsigns += 1

                    # Calculate accuracy (rpval)
                    pred_rpval = out_rpval[mask_succeed]
                    label_rpval = y_rpval[mask_succeed]
                    if pred_rpval.size(0) != pred_jumps.size(0):
                        raise ValueError('Misaligned values')
                    # Compare vectors
                    for pred_vec, label_vec in zip(pred_rpval, label_rpval):
                        if (torch.abs(pred_vec - label_vec) <= 0.3 * torch.abs(label_vec)):
                            correct_rpvals += 1

                    # Calculate accuracy (rpsign)
                    # First, convert the 2 bits classification to 1 bit number
                    pred_rule = (out_rule[mask_succeed].reshape(-1, 2).max(dim=1)[1])
                    label_rule = (y_rule[mask_succeed].reshape(-1, 2).max(dim=1)[1])
                    if pred_rule.size(0) != pred_jumps.size(0):
                        raise ValueError('Misaligned values')
                    # Compare vectors
                    for pred_vec, label_vec in zip(pred_rule, label_rule):
                        if torch.equal(pred_vec, label_vec):
                            correct_rules += 1




            if (self.config.pred in ['position','advance','reward','solo']):
                indomain_avg_loss = total_indomain_loss/len(eval_loader)
                indomain_accuracy =0 if (total_indomain == 0) else correct_indomain/total_indomain
                succeed_avg_loss = total_succeed_loss/len(eval_loader)
                succeed_accuracy = 0 if (total_succeed == 0) else correct_succeed/total_succeed
                jumps_avg_loss = total_jumps_loss / len(eval_loader)
                jumps_accuracy = 0 if (total_jumps == 0) else correct_jumps / total_jumps
                advance_avg_loss = total_advance_loss / len(eval_loader)
                advance_accuracy = 0 if (total_jumps == 0) else correct_advances / total_jumps
                rpsign_avg_loss = total_rpsign_loss / len(eval_loader)
                rpsign_accuracy = 0 if (total_jumps == 0) else correct_rpsigns / total_jumps
                rpval_avg_loss = total_rpval_loss / len(eval_loader)
                rpval_accuracy = 0 if (total_jumps == 0) else correct_rpvals / total_jumps
                rule_avg_loss = total_rule_loss / len(eval_loader)
                rule_accuracy = 0 if (total_jumps == 0) else correct_rules / total_jumps
                

    
                print(f"Evaluation:\n"
                    f"  InDomain Loss: {indomain_avg_loss:.4f}, InDomain Accuracy: {indomain_accuracy:.4f}\n"
                    f"  Succeed Loss: {succeed_avg_loss:.4f}, Succeed Accuracy: {succeed_accuracy:.4f}\n"
                    f"  Jump Loss: {jumps_avg_loss:.4f}, Jump Accuracy: {jumps_accuracy:.4f}\n"
                    f"  Advance Loss: {advance_avg_loss:.4f}, Advance Accuracy: {advance_accuracy:.4f}\n"
                    f"  Rpsign Loss: {rpsign_avg_loss:.4f}, Rpsign Accuracy: {rpsign_accuracy:.4f}\n"
                    f"  Rpval Loss: {rpval_avg_loss:.4f}, Rpval Accuracy: {rpval_accuracy:.4f}\n"
                    f"  Rule Loss: {rule_avg_loss:.4f}, Rule Accuracy: {rule_accuracy:.4f}\n"
                    )

                self.performance = {
                    'indomain_loss': indomain_avg_loss, 
                    'indomain_accuracy': indomain_accuracy, 
                    'succeed_loss': succeed_avg_loss, 
                    'succeed_accuracy': succeed_accuracy, 
                    'jumps_loss': jumps_avg_loss, 
                    'jumps_accuracy': jumps_accuracy, 
                    'advance_loss': advance_avg_loss, 
                    'advance_accuracy': advance_accuracy, 
                    'rpsign_loss': rpsign_avg_loss, 
                    'rpsign_accuracy': rpsign_accuracy, 
                    'rpval_loss': rpval_avg_loss, 
                    'rpval_accuracy': rpval_accuracy,
                    'rule_loss': rule_avg_loss, 
                    'rule_accuracy': rule_accuracy
                }
                return self.performance
            
        
    
    def save_model(self, model_path: str):
        # Ensure the directory exists
        directory = os.path.dirname(model_path)
        if directory!="" and not os.path.exists(directory):
            os.makedirs(directory)
        # Save the model state, optimizer state, and scheduler state
        model_data = {
            'state_dict': self.state_dict(),
            'optimizer_state': self.optimizer.state_dict(),
            'scheduler_state': self.scheduler.state_dict(),
            'performance': getattr(self, 'performance', None),  # Include performance metrics if available
            'config':self.config
        }
        torch.save(model_data, model_path)
        
    def load_model(self, model_path: str):
        # Load the model data from the file
        model_data = torch.load(model_path)
        self.load_state_dict(model_data['state_dict'])
        
        # Restore optimizer and scheduler states
        if 'optimizer_state' in model_data:
            if self.optimizer == None:
                self.optimizer = optim.Adam(self.parameters(), lr=self.config.learning_rate)
            self.optimizer.load_state_dict(model_data['optimizer_state'])
        if 'scheduler_state' in model_data:
            if self.scheduler == None:
                self.scheduler = ReduceLROnPlateau(self.optimizer, mode='min', factor=0.1, patience=10,threshold=1e-3, cooldown= 5, min_lr= 1e-5)
            self.scheduler.load_state_dict(model_data['scheduler_state'])
            # Manually reset internal counters to prevent immediate learning rate drop
            self.scheduler.last_epoch = -1
            self.scheduler.num_bad_epochs = 0
            self.scheduler.cooldown_counter = 0
            self.scheduler.best = float('inf') if self.scheduler.mode == 'min' else -float('inf')
            self.scheduler.mode_worse = float('inf') if self.scheduler.mode == 'min' else -float('inf')
        # (optional) Restore performance information, if available
        if 'performance' in model_data:
            self.performance = model_data['performance']
        if 'config' in model_data:
            #reset epochs
            epochs = self.config.epochs
            self.config = model_data['config']
            self.config.epochs = epochs
            
    def predict(self, graph:Data):
        self.eval()  # Set the model to evaluation mode
        with torch.no_grad():
            out = self.forward(graph.to(device))  # Run forward pass on single graph
            if self.config.pred in ['position','advance','reward','solo']:
                out_indomain = out[:,:2].view(-1,2)
                out_succeed = out[:,2:2+2].view(-1,2)
                out_jumps = out[:,2+2:2+2+max_ast_depth*3].view(-1,max_ast_depth*3)
                out_advance = out[:,2+2+max_ast_depth*3]
                out_rpsign = out[:,2+2+max_ast_depth*3 +1:2+2 + max_ast_depth*3 +1 +2].view(-1,2)
                out_rpval = out[:,2+2+max_ast_depth*3 +1 +2]
                out_rule = out[:,2+2+max_ast_depth*3 +1+2+1:2+2 + max_ast_depth*3 +1 +2+1+2].view(-1,2)



                pred_indomain = out_indomain.argmax(dim=1).squeeze(0).item()
                pred_succeed = out_succeed.argmax(dim=1).squeeze(0).item()
                if pred_indomain == 0:
                    pred_succeed == -1
                    pred_jumps = torch.zeros(max_ast_depth).tolist()
                    pred_advance = -1
                    pred_rpsign = -1
                    pred_rpval = 0
                    pred_rule = -1
                else:
                    if pred_succeed == 0:
                        pred_jumps = torch.zeros(max_ast_depth).tolist()
                        pred_advance = -1
                        pred_rpsign = -1
                        pred_rpval = 0
                        pred_rule = -1
                    else:
                        pred_jumps = (out_jumps.view(-1, 3).argmax(dim=1) - 1).view(-1, max_ast_depth).squeeze(0).tolist()
                        pred_advance = round(out_advance.squeeze(0).item())
                        pred_rpsign = out_rpsign.argmax(dim=1).squeeze(0).item()
                        pred_rpval = out_rpval.squeeze(0).item()
                        pred_rule = out_rule.argmax(dim=1).squeeze(0).item()
                return pred_indomain, pred_succeed, pred_jumps, pred_advance, pred_rpsign, pred_rpval, pred_rule
            else:
                raise ValueError('Unknow config')


        


from graphDataloader import *
if __name__ == "__main__":
    print(f"*****************nn****************")
    # Instantiate DataLoaderConfig
    data_loader_config = DataLoaderConfig(pred='solo',batch_size=16)

    # example_graphs = initialize_from_file(filename="1__family#1.json",pred="solo")
    example_graphs = initialize_from_file(filename="1__quadratic.json",pred="solo") 
    # Assuming you've correctly defined your ASTGraphDataLoader with index file loading/saving:
    dataloader = ASTGraphDataLoader(example_graphs, data_loader_config, 'index_file.pkl')
    train_loader, eval_loader = dataloader.get_data_loaders()
    # Test the __len__ method
    assert (len(train_loader)+ len(eval_loader))* data_loader_config.batch_size >= len(example_graphs), "Length mismatch in ASTGraphDataset."

    print("Tests completed successfully.")
    # Define model configuration
    config = ModelConfig(
        showfig=True, # For visualizing the training process
        epochs=1, #for test
        pred= 'solo',
    )
    
    # Initialize the model
    model = GAT_LSTM_Model(config)

    # Define loss function (assume a classification task)
    criterion = torch.nn.CrossEntropyLoss()

    # Perform model training
    model.train_model(train_loader=train_loader)
    print("Training completed.")
    
    # Evaluate the model
    performance = model.eval_model(eval_loader=eval_loader)
    print(f"Performance: {performance}")
    
    # Save the model
    model.save_model('model_state.mdl')
    print("Model saved.")

    # Load the model
    model.load_model('model_state.mdl')
    print("Model loaded.")

    # Retrain and re-evaluate the model after loading
    model.train_model(train_loader=train_loader)
    print("Retraining completed.")
    
    performance = model.eval_model(eval_loader=eval_loader)
    print(f"Re-evaluation performance: {performance}")

