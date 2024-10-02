from modelPool import ModelWrapper
from torch.utils.data import DataLoader
from torch.utils.data.dataset import random_split
from torch.autograd import Function
import matplotlib.pyplot as plt
from extractTrainingDataFromJsonFile import *
from embeddingTrainingData import *
import numpy as np


# for training
import torch
import torch.nn as nn
from torch.utils.data import Dataset, DataLoader, SequentialSampler
from torch.utils.data import DataLoader, WeightedRandomSampler, TensorDataset
import torch.optim as optim
import torch.nn.functional as F

from coupledmodels import *

import os
import threading
# experimental standard configs for models, all control variables are the same: 
position_model_config = ModelConfig(
        embedding_sizes={'boundtfdomain':50,'type': 50, 'identifier': 1000, 'string': 1000, 'operator': 40},
        gat_heads=8,
        hidden_lstm_features=128,
        num_lstm_layer=2,
        learning_rate=0.001,
        step_size=30,#useless now
        gamma=0.1,
        showfig=False, # For visualizing the training process
        epochs=100, #for test
        pred= 'position',
        batch_size=32,
    )
advance_model_config = ModelConfig(
        embedding_sizes={'boundtfdomain':50,'type': 50, 'identifier': 1000, 'string': 1000, 'operator': 40},
        gat_heads=8,
        hidden_lstm_features=128,
        num_lstm_layer=2,
        learning_rate=0.001,
        step_size=30,#useless now
        gamma=0.1,
        showfig=False, # For visualizing the training process
        epochs=100, #for test
        pred= 'advance',
        batch_size=32,

    )
reward_model_config = ModelConfig(
        embedding_sizes={'boundtfdomain':50,'type': 50, 'identifier': 1000, 'string': 1000, 'operator': 40},
        gat_heads=8,
        hidden_lstm_features=128,
        num_lstm_layer=2,
        learning_rate=0.001,
        step_size=30,#useless now
        gamma=0.1,
        showfig=False, # For visualizing the training process
        epochs=100, #for test
        pred= 'reward',
        batch_size=32,
    )
solo_model_config = ModelConfig(
        embedding_sizes={'boundtfdomain':50,'type': 50, 'identifier': 1000, 'string': 1000, 'operator': 40},
        gat_heads=8,
        hidden_lstm_features=128,
        num_lstm_layer=2,
        learning_rate=0.001,
        step_size=30,#useless now
        gamma=0.1,
        showfig=False, # For visualizing the training process
        epochs=100, #for test
        pred= 'solo',
        batch_size=32,
    )

position_data_loader_config = DataLoaderConfig(pred='position')
advance_data_loader_config = DataLoaderConfig(pred='advance')
reward_data_loader_config = DataLoaderConfig(pred='reward')
solo_data_loader_config = DataLoaderConfig(pred='solo')
filter_config = True
multithread_config = False



class GuidedReasoning(ModelWrapper):
    def __init__(self, filter = filter_config, multithread = multithread_config) -> None:
        '''
        if filter is True, then the logic reasoning will use coupled model, else, use solo model
        '''
        self.filter = filter
        self.multithread = multithread
        self.position_model = None
        self.advance_model = None
        self.reward_model = None
        self.solo_model = None
        self.incorrect_preds_filtered = 0
        self.total_preds_count = 0


        self.position_data_loader_config = None
        self.advance_data_loader_config = None
        self.reward_data_loader_config = None
        self.solo_data_loader_config = None
        if filter == True:
            self.position_model = GAT_LSTM_Model(config=position_model_config)
            self.advance_model = GAT_LSTM_Model(config=advance_model_config)
            self.reward_model = GAT_LSTM_Model(config=reward_model_config)

            self.position_data_loader_config = position_data_loader_config
            self.advance_data_loader_config = advance_data_loader_config
            self.reward_data_loader_config = reward_data_loader_config


        elif filter == False:
            self.solo_model = GAT_LSTM_Model(config=solo_model_config)

            self.solo_data_loader_config = solo_data_loader_config

        pass

    def train(
        self, trainingDataFilePath=None, trainingDataList: list = None
    ) -> tuple[str, str]:
        if (self.filter == True):
            if (self.multithread == True):
                threads = []
                def train_model_thread(model, train_loader, eval_loader):
                    model.train_model(train_loader)
                    model.eval_model(eval_loader)
                # Position
                position_graphs = initialize_from_file(filename=trainingDataFilePath, pred="position")
                position_dataloader = ASTGraphDataLoader(position_graphs, self.position_data_loader_config, 'index_file.pkl')
                position_train_loader, position_eval_loader = position_dataloader.get_data_loaders()
                
                # Create a thread for position model training and evaluation
                position_thread = threading.Thread(target=train_model_thread, args=(self.position_model, position_train_loader, position_eval_loader))
                threads.append(position_thread)

                # Advance
                advance_graphs = initialize_from_file(filename=trainingDataFilePath, pred="advance")
                advance_dataloader = ASTGraphDataLoader(advance_graphs, self.advance_data_loader_config, 'index_file.pkl')
                advance_train_loader, advance_eval_loader = advance_dataloader.get_data_loaders()
                
                # Create a thread for advance model training and evaluation
                advance_thread = threading.Thread(target=train_model_thread, args=(self.advance_model, advance_train_loader, advance_eval_loader))
                threads.append(advance_thread)

                # Reward
                reward_graphs = initialize_from_file(filename=trainingDataFilePath, pred="reward")
                reward_dataloader = ASTGraphDataLoader(reward_graphs, self.reward_data_loader_config, 'index_file.pkl')
                reward_train_loader, reward_eval_loader = reward_dataloader.get_data_loaders()
                
                # Create a thread for reward model training and evaluation
                reward_thread = threading.Thread(target=train_model_thread, args=(self.reward_model, reward_train_loader, reward_eval_loader))
                threads.append(reward_thread)

                # Start all threads
                for thread in threads:
                    thread.start()

                # Wait for all threads to complete
                for thread in threads:
                    thread.join()

                # Gather and print or return results
                self.indomain_accuracy = self.position_model.performance.get('indomain_accuracy',None)
                self.succeed_accuracy = self.position_model.performance.get('succeed_accuracy',None)
                self.policies_accuracy = self.position_model.performance.get('jumps_accuracy', None)
                self.advance_accuracy = self.advance_model.performance.get('advance_accuracy', None)
                self.rpsign_accuracy = self.reward_model.performance.get('rpsign_accuracy', None)
                self.rpval_accuracy = self.reward_model.performance.get('rpval_accuracy', None)
                self.rule_accuracy = self.reward_model.performance.get('rule_accuracy', None)
                
                try:
                    torch.cuda.empty_cache()
                except Exception:
                    pass
            else:
                # position
                position_graphs = initialize_from_file(filename=trainingDataFilePath,pred="position") 
                position_dataloader = ASTGraphDataLoader(position_graphs, self.position_data_loader_config, 'index_file.pkl')
                position_train_loader, position_eval_loader = position_dataloader.get_data_loaders()
                self.position_model.train_model(position_train_loader)
                self.position_model.eval_model(position_eval_loader)
                self.indomain_accuracy = self.position_model.performance.get('indomain_accuracy',None)
                self.succeed_accuracy = self.position_model.performance.get('succeed_accuracy',None)
                self.policies_accuracy = self.position_model.performance.get('jumps_accuracy',None)
                try:
                    torch.cuda.empty_cache()
                except Exception:
                    pass
                # advance
                advance_graphs = initialize_from_file(filename=trainingDataFilePath,pred="advance") 
                advance_dataloader = ASTGraphDataLoader(advance_graphs, self.advance_data_loader_config, 'index_file.pkl')
                advance_train_loader, advance_eval_loader = advance_dataloader.get_data_loaders()
                self.advance_model.train_model(advance_train_loader)
                self.advance_model.eval_model(advance_eval_loader)
                self.advance_accuracy = self.advance_model.performance.get('advance_accuracy',None)
                try:
                    torch.cuda.empty_cache()
                except Exception:
                    pass
                # reward
                reward_graphs = initialize_from_file(filename=trainingDataFilePath,pred="reward") 
                reward_dataloader = ASTGraphDataLoader(reward_graphs, self.reward_data_loader_config, 'index_file.pkl')
                reward_train_loader, reward_eval_loader = reward_dataloader.get_data_loaders()
                self.reward_model.train_model(reward_train_loader)
                self.reward_model.eval_model(reward_eval_loader)
                self.rpsign_accuracy = self.reward_model.performance.get('rpsign_accuracy',None)
                self.rpval_accuracy = self.reward_model.performance.get('rpval_accuracy',None)
                self.rule_accuracy = self.reward_model.performance.get('rule_accuracy',None)

                try:
                    torch.cuda.empty_cache()
                except Exception:
                    pass
        else:
            #solo
            solo_graphs = initialize_from_file(filename=trainingDataFilePath,pred="solo") 
            solo_dataloader = ASTGraphDataLoader(solo_graphs, self.solo_data_loader_config, 'index_file.pkl')
            solo_train_loader, solo_eval_loader = solo_dataloader.get_data_loaders()
            self.solo_model.train_model(solo_train_loader)
            self.solo_model.eval_model(solo_eval_loader)
            
            self.indomain_accuracy = self.solo_model.performance.get('indomain_accuracy',None)
            self.succeed_accuracy = self.solo_model.performance.get('succeed_accuracy',None)
            self.policies_accuracy = self.solo_model.performance.get('jumps_accuracy',None)
            self.advance_accuracy = self.solo_model.performance.get('advance_accuracy',None)
            self.rpsign_accuracy = self.solo_model.performance.get('rpsign_accuracy',None)
            self.rpval_accuracy = self.solo_model.performance.get('rpval_accuracy',None)
            self.rule_accuracy = self.solo_model.performance.get('rule_accuracy',None)

            try:
                torch.cuda.empty_cache()
            except Exception:
                pass
        return self.indomain_accuracy, self.succeed_accuracy, self.policies_accuracy, self.advance_accuracy, self.rpsign_accuracy, self.rpval_accuracy, self.rule_accuracy
    def load_model(self, model_path: str):
        if self.filter == True:
            self.position_model.load_model(model_path=model_path+'.position')
            self.advance_model.load_model(model_path=model_path+'.advance')
            self.reward_model.load_model(model_path=model_path+'.reward')
            self.indomain_accuracy = self.position_model.performance.get('indomain_accuracy',None)
            self.succeed_accuracy = self.position_model.performance.get('succeed_accuracy',None)
            self.policies_accuracy = self.position_model.performance.get('jumps_accuracy',None)
            self.advance_accuracy = self.advance_model.performance.get('advance_accuracy',None)
            self.rpsign_accuracy = self.reward_model.performance.get('rpsign_accuracy',None)
            self.rpval_accuracy = self.reward_model.performance.get('rpval_accuracy',None)
            self.rule_accuracy = self.reward_model.performance.get('rule_accuracy',None)

        else:
            self.solo_model.load_model(model_path=model_path+'.solo')
            self.indomain_accuracy = self.solo_model.performance.get('indomain_accuracy',None)
            self.succeed_accuracy = self.solo_model.performance.get('succeed_accuracy',None)
            self.policies_accuracy = self.solo_model.performance.get('jumps_accuracy',None)
            self.advance_accuracy = self.solo_model.performance.get('advance_accuracy',None)
            self.rpsign_accuracy = self.solo_model.performance.get('rpsign_accuracy',None)
            self.rpval_accuracy = self.solo_model.performance.get('rpval_accuracy',None)
            self.rule_accuracy = self.solo_model.performance.get('rule_accuracy',None)



    def predict(
        self, grounding_state_str: str, predicted_position_output_file: str, nn_knowledge_domain = ""
    ):
        self.total_preds_count += 1
        json_result = {}
        
            
        if (self.filter == True):
            '''
            Use coupled networks with error enhancing and check ability for filtering
            '''
            json_result1 = {} #posiiton nn
            json_result2 = {} #advance nn
            json_result3 = {} #reward nn
            pred_extended_grounding_state = ExtendedGroundingState(
            GroundingState(**json.loads(grounding_state_str))
            )
            pred_extended_grounding_state.setGsDomain(nn_knowledge_domain)
            # position
            position_astNetworkXGraph = ASTNetworkXGraph(pred_extended_grounding_state,pred='position')
            
            pred_position_graphs = [position_astNetworkXGraph]
            if getattr(self,'pred_dataloader', None) == None:
                self.pred_dataloader = ASTGraphDataLoader(ast_graphs=pred_position_graphs, \
                                                                   config=DataLoaderConfig(batch_size = 1,train_eval_split=0, pred='position'))
            else:
                self.pred_dataloader.update_graphs(ast_graphs=pred_position_graphs,\
                                                   config=DataLoaderConfig(batch_size = 1,train_eval_split=0, pred='position'))
                
            _, position_pred_loader = self.pred_dataloader.get_data_loaders()
            position_graph:Data = next(iter(position_pred_loader)) #pyg data

            pred_indomain1, pred_succeed1, ast_position1, pred_advance1, pred_rpsign1, pred_rpval1, pred_rule1 = self.position_model.predict(position_graph) #get max_ast_depth bit ast position
            tac_position1 = position_astNetworkXGraph.sat_tac_posmap.get_tac_position(ast_position1)

            json_result1['indomain'] = pred_indomain1
            json_result1['ac_indomain'] = self.indomain_accuracy
            json_result1['succeed'] = pred_succeed1
            json_result1['ac_succeed'] = self.succeed_accuracy
            json_result1['policies'] = tac_position1
            json_result1['ac_policies'] = self.policies_accuracy
            json_result1['advance'] = pred_advance1
            json_result1['ac_advance'] = self.advance_accuracy
            json_result1['rpsign'] = pred_rpsign1
            json_result1['ac_rpsign'] = self.rpsign_accuracy
            json_result1['rpval'] = pred_rpval1
            json_result1['ac_rpval'] = self.rpval_accuracy
            json_result1['rule'] = pred_rule1
            json_result1['ac_rule'] = self.rule_accuracy


            # advance
            advance_astNetworkXGraph = ASTNetworkXGraph(pred_extended_grounding_state, applied_jumps= ast_position1,pred='advance')
            
            pred_advance_graphs = [advance_astNetworkXGraph]
            if getattr(self,'pred_dataloader', None) == None:
                self.pred_dataloader = ASTGraphDataLoader(ast_graphs=pred_advance_graphs, \
                                                                   config=DataLoaderConfig(batch_size = 1,train_eval_split=0, pred='advance'))
            else:
                self.pred_dataloader.update_graphs(ast_graphs=pred_advance_graphs,\
                                                   config=DataLoaderConfig(batch_size = 1,train_eval_split=0, pred='advance'))
                
            _, advance_pred_loader = self.pred_dataloader.get_data_loaders()
            advance_graph:Data = next(iter(advance_pred_loader)) #pyg data

            pred_indomain2, pred_succeed2, ast_position2, pred_advance2, pred_rpsign2, pred_rpval2, pred_rule2  = self.advance_model.predict(advance_graph) #get max_ast_depth bit ast position
            tac_position2 = advance_astNetworkXGraph.sat_tac_posmap.get_tac_position(ast_position2)

            

            json_result2['indomain'] = pred_indomain2
            json_result2['ac_indomain'] = self.indomain_accuracy
            json_result2['succeed'] = pred_succeed2
            json_result2['ac_succeed'] = self.succeed_accuracy
            json_result2['policies'] = tac_position2
            json_result2['ac_policies'] = self.policies_accuracy
            json_result2['advance'] = pred_advance2
            json_result2['ac_advance'] = self.advance_accuracy
            json_result2['rpsign'] = pred_rpsign2
            json_result2['ac_rpsign'] = self.rpsign_accuracy
            json_result2['rpval'] = pred_rpval2
            json_result2['ac_rpval'] = self.rpval_accuracy
            json_result2['rule'] = pred_rule2
            json_result2['ac_rule'] = self.rule_accuracy


            # reward
            reward_astNetworkXGraph = ASTNetworkXGraph(pred_extended_grounding_state,advance= json_result2['advance'], applied_jumps= ast_position1,pred='reward')
            
            pred_reward_graphs = [reward_astNetworkXGraph]
            if getattr(self,'pred_dataloader', None) == None:
                self.pred_dataloader = ASTGraphDataLoader(ast_graphs=pred_reward_graphs, \
                                                                   config=DataLoaderConfig(batch_size = 1,train_eval_split=0, pred='reward'))
            else:
                self.pred_dataloader.update_graphs(ast_graphs=pred_reward_graphs,\
                                                   config=DataLoaderConfig(batch_size = 1,train_eval_split=0, pred='reward'))
                
            _, reward_pred_loader = self.pred_dataloader.get_data_loaders()
            reward_graph:Data = next(iter(reward_pred_loader)) #pyg data

            pred_indomain3, pred_succeed3, ast_position3, pred_advance3, pred_rpsign3, pred_rpval3, pred_rule3 = self.reward_model.predict(reward_graph) #get max_ast_depth bit ast position
            tac_position3  = reward_astNetworkXGraph.sat_tac_posmap.get_tac_position(ast_position3)

            json_result3['indomain'] = pred_indomain3
            json_result3['ac_indomain'] = self.indomain_accuracy
            json_result3['succeed'] = pred_succeed3
            json_result3['ac_succeed'] = self.succeed_accuracy
            json_result3['policies'] = tac_position3
            json_result3['ac_policies'] = self.policies_accuracy
            json_result3['advance'] = pred_advance3
            json_result3['ac_advance'] = self.advance_accuracy
            json_result3['rpsign'] = pred_rpsign3
            json_result3['ac_rpsign'] = self.rpsign_accuracy
            json_result3['rpval'] = pred_rpval3
            json_result3['ac_rpval'] = self.rpval_accuracy
            json_result3['rule'] = pred_rule3
            json_result3['ac_rule'] = self.rule_accuracy

            # Cross-check criteria
            if pred_indomain1 == pred_indomain2 == pred_indomain3:
                json_result['indomain'] = pred_indomain1
                if pred_indomain1 == 0:
                    json_result['succeed'] = -1
                    json_result['policies'] = -1
                    json_result['advance'] = -1
                    json_result['rpsign'] = -1
                    json_result['rpval'] = 0
                    json_result['rule'] = -1

                else:    
                    if pred_succeed1 == pred_succeed2 == pred_succeed3:
                        json_result['succeed'] = pred_succeed1
                        if pred_succeed1 == 0:  # Failure case
                            json_result['policies'] = -1
                            json_result['advance'] = -1
                            json_result['rpsign'] = -1
                            json_result['rpval'] = 0
                            json_result['rule'] = -1
                            
                        else:  # Succeed case
                            if (tac_position1 == tac_position2 == tac_position3 and
                                pred_advance1 == pred_advance2 == pred_advance3 and
                                pred_rpsign1 == pred_rpsign2 == pred_rpsign3 and
                                pred_rule1 == pred_rule2 == pred_rule3 and
                                (pred_rpsign1 == 1 and pred_rpval1 > 0 and pred_rpval2 > 0 and pred_rpval3 > 0 or
                                pred_rpsign1 == 0 and pred_rpval1 < 0 and pred_rpval2 < 0 and pred_rpval3 < 0) and
                                (min(pred_rpval1,pred_rpval2, pred_rpval3)/max(pred_rpval1,pred_rpval2, pred_rpval3)>0.9)
                                ):
                                json_result['policies'] = tac_position1
                                json_result['advance'] = pred_advance1
                                json_result['rpsign'] = pred_rpsign1
                                json_result['rpval'] = pred_rpval1
                                json_result['rule'] = pred_rule1
                            else:
                                json_result['indomain'] = -1
                                json_result['succeed'] = -1
                                json_result['policies'] = -1
                                json_result['advance'] = -1
                                json_result['rpsign'] = -1
                                json_result['rpval'] = 0
                                json_result['rule'] = -1
                                self.incorrect_preds_filtered+=1
                    else:
                        json_result['indomain'] = -1
                        json_result['succeed'] = -1
                        json_result['policies'] = -1
                        json_result['advance'] = -1
                        json_result['rpsign'] = -1
                        json_result['rpval'] = 0
                        json_result['rule'] = -1
                        self.incorrect_preds_filtered+=1
            else:
                json_result['indomain'] = -1
                json_result['succeed'] = -1
                json_result['policies'] = -1
                json_result['advance'] = -1
                json_result['rpsign'] = -1
                json_result['rpval'] = 0
                json_result['rule'] = -1
                self.incorrect_preds_filtered+=1

            # Optionally, add accuracy metrics if needed
            json_result['ac_indomain'] = self.indomain_accuracy
            json_result['ac_succeed'] = self.succeed_accuracy
            json_result['ac_policies'] = self.policies_accuracy
            json_result['ac_advance'] = self.advance_accuracy
            json_result['ac_rpsign'] = self.rpsign_accuracy
            json_result['ac_rpval'] = self.rpval_accuracy
            json_result['ac_rule'] = self.rule_accuracy

            return json_result
            
        elif self.filter == False:
            '''
            Use a single network with weak filter ability
            '''
            pred_extended_grounding_state = ExtendedGroundingState(
            GroundingState(**json.loads(grounding_state_str))
            )
            # solo
            solo_astNetworkXGraph = ASTNetworkXGraph(pred_extended_grounding_state,pred='solo')
            
            pred_solo_graphs = [solo_astNetworkXGraph]
            if getattr(self,'pred_dataloader', None) == None:
                self.pred_dataloader = ASTGraphDataLoader(ast_graphs=pred_solo_graphs, \
                                                                   config=DataLoaderConfig(batch_size = 1,train_eval_split=0, pred='solo'))
            else:
                self.pred_dataloader.update_graphs(ast_graphs=pred_solo_graphs,\
                                                   config=DataLoaderConfig(batch_size = 1,train_eval_split=0, pred='solo'))
                
            _, solo_pred_loader = self.pred_dataloader.get_data_loaders()
            solo_graph:Data = next(iter(solo_pred_loader)) #pyg data

            pred_indomain, pred_succeed, ast_position, pred_advance, pred_rpsign, pred_rpval, pred_rule = self.solo_model.predict(solo_graph) #get max_ast_depth bit ast position
            tac_position = solo_astNetworkXGraph.sat_tac_posmap.get_tac_position(ast_position)
            if tac_position is None or tac_position < 0:
                pred_indomain = 0

            # Check criteria
            json_result['indomain'] = pred_indomain
            if pred_indomain == 1:
                json_result['succeed'] = pred_succeed
                if pred_succeed == 1:  # Succeed case
                    # Validate 'policies', 'advance', 'rpsign', and 'rpval'
                    if pred_rpval > 0 and pred_rpsign == 1 or pred_rpval < 0 and pred_rpsign == 0:
                        json_result['policies'] = tac_position
                        json_result['advance'] = pred_advance
                        json_result['rpsign'] = pred_rpsign
                        json_result['rpval'] = pred_rpval
                        json_result['rule'] = pred_rule
                    else:
                        json_result['indomain'] = -1
                        json_result['succeed'] = -1
                        json_result['policies'] = -1
                        json_result['advance'] = -1
                        json_result['rpsign'] = -1
                        json_result['rpval'] = 0
                        json_result['rule'] = -1
                        self.incorrect_preds_filtered+=1
                else:  # Failure case
                    json_result['policies'] = -1
                    json_result['advance'] = -1
                    json_result['rpsign'] = -1
                    json_result['rpval'] = 0
                    json_result['rule'] = -1
            else:
                json_result['succeed'] = -1
                json_result['policies'] = -1
                json_result['advance'] = -1
                json_result['rpsign'] = -1
                json_result['rpval'] = 0
                json_result['rule'] = -1
            # Add accuracy metrics
            json_result['ac_indomain'] = self.indomain_accuracy
            json_result['ac_succeed'] = self.succeed_accuracy
            json_result['ac_policies'] = self.policies_accuracy
            json_result['ac_advance'] = self.advance_accuracy
            json_result['ac_rpsign'] = self.rpsign_accuracy
            json_result['ac_rpval'] = self.rpval_accuracy
            json_result['ac_rule'] = self.rule_accuracy

        return json_result
    def get_state_dict(self):
        """Equivalent to calling model.state_dict(), return state dict"""
        raise ModuleNotFoundError('This function is deprecated.')

    def load_state_dict(self, state_dict):
        raise ModuleNotFoundError('This function is deprecated.')

    def save_model(self, model_path: str):
        """Save the current model to the provided model path."""
        if self.filter == True:
            self.position_model.save_model(model_path=model_path+'.position')
            self.advance_model.save_model(model_path=model_path+'.advance')
            self.reward_model.save_model(model_path=model_path+'.reward')
            
        else:
            self.solo_model.save_model(model_path=model_path+'.solo')
