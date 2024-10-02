import json
import random
from collections import defaultdict
import matplotlib.pyplot as plt
import inspect

max_ast_depth = 16

def current_pos():
    print("file:///" + __file__ + "#" +
          str(inspect.currentframe().f_back.f_lineno))


# Extract GroundingState entries from json file.


# Defining the classes
class Operand:
    def __init__(
        self, argName="", argType="", changeable=False, className="", isClass=False
    ):
        self.argName = argName
        self.argType = argType
        self.changeable = changeable
        self.className = className
        self.isClass = isClass


class Operator:
    def __init__(self, argName="", argType=""):
        self.argName = argName
        self.argType = argType


class Result(Operand):
    pass


class Code:
    def __init__(
        self, grounded=False, boundtfdomain = "", root = False, operand1={}, operand2={}, operator={}, result={}
    ):
        self.grounded = grounded
        self.boundtfdomain = boundtfdomain
        self.root = root    
        self.operand1 = Operand(**operand1)
        self.operand2 = Operand(**operand2)
        self.operator = Operator(**operator)
        self.result = Result(**result)


class CodeTable:
    def __init__(self, codeTable):
        # If codeTable is a CodeTable instance, extract the actual list of codeList
        if isinstance(codeTable, CodeTable):
            self.codeList = codeTable.codeList
        else:
            self.codeList = [Code(**code) for code in codeTable]

class PositionMapping:
    def __init__(self):
        # Initialize an empty dictionary to hold the mapping
        # Keys are tuples representing AST positions of length max_ast_depth
        # Values are integers representing TAC positions
        self.mapping = {}
        self.reversemapping = {}
    
    def _ensure_length(self, ast_position):
        """
        Ensure the AST position list is of length max_ast_depth, pad with zeros if necessary.
        
        Parameters:
        - ast_position (list): The AST position list
        
        Returns:
        - A tuple of length max_ast_depth derived from the input list
        """
        # Pad the list with zeros to ensure length max_ast_depth
        padded_position = (ast_position + [0]*max_ast_depth)[:max_ast_depth]
        return tuple(padded_position)
    
    def add_mapping(self, ast_position, tac_position):
        """
        Add a new mapping from an AST position to a TAC position.
        
        Parameters:
        - ast_position (list): The AST position as a list
        - tac_position (int): The corresponding TAC position
        """
        # Ensure the AST position is a list
        if not isinstance(ast_position, list):
            raise ValueError("AST position must be provided as a list.")
        
        # Ensure the AST position list is of length max_ast_depth
        ast_position_key = self._ensure_length(ast_position)
        
        # Add or update the mapping
        self.mapping[ast_position_key] = tac_position
        self.reversemapping[tac_position] = ast_position_key
    
    def get_tac_position(self, ast_position):
        """
        Retrieve the TAC position given an AST position.
        
        Parameters:
        - ast_position (list): The AST position as a list
        
        Returns:
        - The corresponding TAC position (int), or None if not found
        """
        # Ensure the AST position list is of the required length
        ast_position_key = self._ensure_length(ast_position)
        
        return self.mapping.get(ast_position_key, None)
    
    def get_ast_position(self, tac_position):
        """
        Retrieve the AST position given an TAC position.
        
        Parameters:
        - The TAC position (int), or None if not found
        
        Returns:
        - ast_position (list): The AST position as a list
        """
        # Ensure the AST position list is of the required length
        
        return self.reversemapping.get(tac_position, None)

class GroundingState:
    def __init__(
        self,
        codeTable=[],
        energy=0.0,
        initialState=False,
        position=0,
        relatedFilesAndClasses=[],
        indomain=False,
        succeed=False,
        step=0,
        advance=0,
        rpsign=0,
        rpval=0,
        rule = False,
        gstfdomain = "",
        domainstepmap = {},
        **kwargs
    ):
        self.codeTable = CodeTable(codeTable)
        self.energy = energy
        self.initialState = initialState
        self.position = position
        self.relatedFilesAndClasses = relatedFilesAndClasses
        self.indomain = indomain
        self.succeed = (succeed if indomain else 0)
        self.step = step
        self.advance = advance
        self.rpsign = rpsign
        self.rpval = rpval
        self.rule = rule
        self.gstfdomain = gstfdomain
        self.domainstepmap = domainstepmap
        


# import json
# from collections import defaultdict

# Base classes
# ... [All previously defined classes here, such as Operand, Code, etc.] ...

# Extended classes
class ExtendedOperand(Operand):
    def __init__(self, argName, argType, changeable=False, className="", isClass="0"):
        super().__init__(argName, argType, changeable, className, isClass)
        self.occurrenceNumber = 0
        self.isLeafNode = False
        self.positionInAST = []
        self.argValue = None  # New member for storing numeric values

        # Check if argType is 'number' and handle the argName and argValue assignment
        if self.argType == "number":
            self.argValue = float(self.argName)
            self.argName = None

    def isEmpty(self):
        return self.argType == "" and self.className == ""
    # Override the equality operator
    def __eq__(self, other):
        if isinstance(other, ExtendedOperand):
            return (
                self.argName == other.argName
                and self.argType == other.argType
                and self.changeable == other.changeable
                and self.className == other.className
                and self.isClass == other.isClass
                and self.argValue == other.argValue
            )
        return False


    # It's good practice to override the not equal operator when overriding the equality operator
    def __ne__(self, other):
        return not self.__eq__(other)

    # Implementing the less than operator
    def __lt__(self, other):
        if not isinstance(other, ExtendedOperand):
            return NotImplemented
        # If both operands have argValue, compare based on it
        if self.argValue is not None and other.argValue is not None:
            return self.argValue < other.argValue

        # If only one operand has argValue, the one with it is considered "smaller"
        if self.argValue is not None:
            return True
        if other.argValue is not None:
            return False
        # Primary sorting criterion: argName
        if self.argName != other.argName:
            return self.argName < other.argName
        # Secondary sorting criterion: argType
        return self.argType < other.argType


class ExtendedOperator(Operator):
    # Add any extra attributes or methods as needed
    pass


class ExtendedResult(ExtendedOperand):
    pass


class ExtendedCode:
    def __init__(self, code: Code):
        self.grounded = code.grounded
        self.boundtfdomain = code.boundtfdomain
        self.root = code.root
        self.extendedOperand1 = ExtendedOperand(**code.operand1.__dict__)
        self.extendedOperand2 = ExtendedOperand(**code.operand2.__dict__)
        self.extendedOperator = ExtendedOperator(**code.operator.__dict__)
        self.extendedResult = ExtendedResult(**code.result.__dict__)

    def get_positionInAST(self):
        return self.extendedResult.positionInAST


class ExtendedCodeTable:
    def __init__(self, codeTable: CodeTable):
        # super().__init__(*args, **kwargs)
        self.extendedCodeList = [ExtendedCode(
            code) for code in codeTable.codeList]
        self.identifierSubstitutionTable = defaultdict(list)
        self.uniqueIdentifiers = set()
        self.pos_map = PositionMapping()
        self.nodeLocalize()
        self.mapPos()
        self.identifierSubstitute()
        self.updateUniqueIdentifiers()
        self.updateOccurrenceNumber()

    def nodeLocalize(self):
        parentNodeSet = set()
        processedNodeSet = {}  # Now a dictionary mapping node name to its position

        rootNode = self.extendedCodeList[-1].extendedResult
        rootNode.positionInAST = []
        if rootNode.argType == "identifier":
            parentNodeSet.add(rootNode.argName)
        processedNodeSet[rootNode.argName] = rootNode.positionInAST

        # Helper function updated to retrieve position from processedNodeSet
        def find_position(arg_name):
            """Helper function to find previously processed positionInAST"""
            return processedNodeSet.get(arg_name)

        # Traverse the list of codeList in reverse
        # We strongly recommend only use at most one assignment symbol in one expression, or this will confuse readers and sometimes be ambiguous.
        for extendedCode in reversed(self.extendedCodeList):
            if extendedCode.extendedResult.argType == "identifier":
                parentNodeSet.add(extendedCode.extendedResult.argName)
            # Check and assign attributes for extendedResult
            if (
                extendedCode.extendedResult.argType == "identifier"
                and extendedCode.extendedResult.argName in processedNodeSet
            ):
                extendedCode.extendedResult.positionInAST = find_position(
                    extendedCode.extendedResult.argName
                )
                extendedCode.extendedResult.isLeafNode = False
            elif (
                extendedCode.extendedResult.argType == "identifier"
                and extendedCode.extendedResult.argName not in processedNodeSet
            ):  # impossible
                extendedCode.extendedResult.positionInAST = []
                extendedCode.extendedResult.isLeafNode = False
                processedNodeSet[
                    extendedCode.extendedResult.argName
                ] = extendedCode.extendedResult.positionInAST

            # Check and assign attributes for extendedOperand1
            extendedCode.extendedOperand1.positionInAST = (
                extendedCode.extendedResult.positionInAST + [-1]
            )
            if extendedCode.extendedOperand1.argType == "identifier":
                processedNodeSet[
                    extendedCode.extendedOperand1.argName
                ] = extendedCode.extendedOperand1.positionInAST

            # Check and assign attributes for extendedOperand2
            extendedCode.extendedOperand2.positionInAST = (
                extendedCode.extendedResult.positionInAST + [1]
            )
            if extendedCode.extendedOperand2.argType == "identifier":
                processedNodeSet[
                    extendedCode.extendedOperand2.argName
                ] = extendedCode.extendedOperand2.positionInAST

        for extendedCode_ in reversed(self.extendedCodeList[::]):
            if (
                extendedCode_.extendedOperand1.argType == "identifier"
                and extendedCode_.extendedOperand1.argName in parentNodeSet
                and extendedCode_.extendedOperator.argType != ""
                and extendedCode_.extendedOperator.argName not in {"=","-->"}
            ):
                extendedCode_.extendedOperand1.isLeafNode = False
            else:
                extendedCode_.extendedOperand1.isLeafNode = True

            if (
                extendedCode_.extendedOperand2.argType == "identifier"
                and extendedCode_.extendedOperand2.argName in parentNodeSet
            ):
                extendedCode_.extendedOperand2.isLeafNode = False
            else:
                extendedCode_.extendedOperand2.isLeafNode = True
    def mapPos(self):
        # Loop through each extended code and map the position in AST
        # with the corresponding line number of TAC.
        for i, extendedCode in enumerate(self.extendedCodeList):
            # Check the extendedCode.extendedResult.positionInAST attribute
            if hasattr(extendedCode, 'extendedResult') and hasattr(extendedCode.extendedResult, 'positionInAST'):
                # Map the AST position to the TAC line number (i)
                ast_position = extendedCode.extendedResult.positionInAST
                self.pos_map.add_mapping(ast_position, i)

    def identifierSubstitute(self):
        # Stub: Implement identifier substitution logic here using identifierSubstitutionTable
        for extendedCode in self.extendedCodeList:
            # Check and replace for extendedOperand1
            if (
                not extendedCode.extendedOperand1.isLeafNode
                and extendedCode.extendedOperand1.argType == "identifier"
            ):
                newPath = (
                    "".join(map(str, extendedCode.extendedOperand1.positionInAST))
                    .replace("-1", "L")
                    .replace("1", "R")
                )
                extendedCode.extendedOperand1.argName = "var" + newPath

            # Check and replace for extendedOperand2
            if (
                not extendedCode.extendedOperand2.isLeafNode
                and extendedCode.extendedOperand2.argType == "identifier"
            ):
                newPath = (
                    "".join(map(str, extendedCode.extendedOperand2.positionInAST))
                    .replace("-1", "L")
                    .replace("1", "R")
                )
                extendedCode.extendedOperand2.argName = "var" + newPath

            # Check and replace for extendedResult
            if (
                not extendedCode.extendedResult.isLeafNode
                and extendedCode.extendedResult.argType == "identifier"
                and extendedCode.extendedOperator.argType != ""
                and extendedCode.extendedOperator.argName not in {"=","-->"}
            ):
                newPath = (
                    "".join(map(str, extendedCode.extendedResult.positionInAST))
                    .replace("-1", "L")
                    .replace("1", "R")
                )
                extendedCode.extendedResult.argName = "var" + newPath

    def updateUniqueIdentifiers(self):
        # Reset the set
        self.uniqueIdentifiers = set()
        for extendedCode in self.extendedCodeList:
            # Check and add for extendedOperand1
            if extendedCode.extendedOperand1.argType == "identifier":
                self.uniqueIdentifiers.add(
                    extendedCode.extendedOperand1.argName)

            # Check and add for extendedOperand2
            if extendedCode.extendedOperand2.argType == "identifier":
                self.uniqueIdentifiers.add(
                    extendedCode.extendedOperand2.argName)

            # Check and add for extendedResult
            if (
                hasattr(extendedCode, "extendedResult")
                and extendedCode.extendedResult.argType == "identifier"
            ):
                self.uniqueIdentifiers.add(extendedCode.extendedResult.argName)

    def updateOccurrenceNumber(self):
        operand_count_dict = {}

        # First pass: Count occurrences
        for extendedCode in self.extendedCodeList:
            # Check and count for extendedOperand1
            if extendedCode.extendedOperand1.argType == "identifier":
                if extendedCode.extendedOperand1.argName not in operand_count_dict:
                    operand_count_dict[extendedCode.extendedOperand1.argName] = 0
                operand_count_dict[extendedCode.extendedOperand1.argName] += 1

            # Check and count for extendedOperand2
            if extendedCode.extendedOperand2.argType == "identifier":
                if extendedCode.extendedOperand2.argName not in operand_count_dict:
                    operand_count_dict[extendedCode.extendedOperand2.argName] = 0
                operand_count_dict[extendedCode.extendedOperand2.argName] += 1

        # Second pass: Assign occurrenceNumber to ExtendedOperands
        for extendedCode in self.extendedCodeList:
            if extendedCode.extendedOperand1.argType == "identifier":
                extendedCode.extendedOperand1.occurrenceNumber = operand_count_dict.get(
                    extendedCode.extendedOperand1.argName, 1
                )
            else:
                extendedCode.extendedOperand1.occurrenceNumber = 1

            if extendedCode.extendedOperand2.argType == "identifier":
                extendedCode.extendedOperand2.occurrenceNumber = operand_count_dict.get(
                    extendedCode.extendedOperand2.argName, 1
                )
            else:
                extendedCode.extendedOperand2.occurrenceNumber = 1


class ExtendedGroundingState:
    def __init__(self, groundingState: GroundingState):
        self.extendedCodeTable = ExtendedCodeTable(groundingState.codeTable)
        self.energy = groundingState.energy
        self.initialState = groundingState.initialState
        self.position = groundingState.position #tac position
        self.relatedFilesAndClasses = groundingState.relatedFilesAndClasses
        self.gstfdomain = groundingState.gstfdomain
        self.domainstepmap = groundingState.domainstepmap
        # predictions
        self.indomain = groundingState.indomain
        self.succeed = groundingState.succeed
        self.onehotPosition = self.getOnehotPosition()
        self.step = groundingState.step
        self.advance = groundingState.advance
        self.rpsign = groundingState.rpsign
        self.rpval = groundingState.rpval
        self.rule = groundingState.rule

    def setGsDomain(self, gsdomain:str=""):
        self.gsdomain = gsdomain
        if (gsdomain!="" and (self.domainstepmap.get(gsdomain) is not None)):
            self.step = self.domainstepmap.get(gsdomain)
        else:
            self.step = 0



    def getOnehotPosition(self):
        # Step 1: Find the code corresponding to ExtendedGroundingState.position in codeTable.
        extendedCode = self.extendedCodeTable.extendedCodeList[self.position]

        # Step 2: Calculate the onehotPosition using code.result.positionInAST
        onehot_val = 0

        # Reverse the positionInAST for the described summation
        # reversed_ast = extendedCode.extendedResult.positionInAST[::-1]

        for i, value in enumerate(extendedCode.extendedResult.positionInAST, start=0):
            if value == 0:
                break
            elif value == -1:  # left child
                flag = 0
            elif value == 1:  # right child
                flag = 1
           
            # Update the onehot value based on the formula
            onehot_val += 2 ** (i + flag)

        return onehot_val

######
# for GNN, convert TAC to Graph Structure.
######
class GraphNode:
    def __init__(self,grounded:bool, boundtfdomain:str, root:bool , changeable:bool,type:str,identifier:str,string:str,number:float,operator:str,
                 step:int = 0, applied:bool = False, jumps:list[int] = [0,0,0,0,0,0,0,0],branch:int = 0,advance:int = 0,
                 **kwargs) -> None:
        self.grounded = grounded #one bit bool
        self.boundtfdomain = boundtfdomain #can be None, string
        self.root = root # one bit bool
        self.changeable = changeable# one bit bool
        self.type = type #can be None, string
        self.identifier = identifier #can be None, string
        self.string = string # can be None, string
        self.number = number # default to be zero, float
        self.operator = operator #default to be None, string
        self.step = step #default to be 0, int
        self.applied = applied# one bit bool
        self.jumps = (jumps + [0]*max_ast_depth)[:max_ast_depth]  # list float, length = max_ast_depth
        self.branch = branch # one bit float, -1 for left, 1 for right, 0 for root
        self.advance = advance# one bit float

    def get_index(self):
        jumps = self.jumps
        index = 0
        for i, branch in enumerate(jumps):
            if branch == 0:
                break
            elif branch == -1:  # left child
                flag = 0
            elif branch == 1:  # right child
                flag = 1

            index += 2 ** (i + flag)

        return index
    @staticmethod
    def jumps_to_index(jumps:list):
        index = 0
        for i, branch in enumerate(jumps):
            if branch == 0:
                break
            elif branch == -1:  # left child
                flag = 0
            elif branch == 1:  # right child
                flag = 1

            index += 2 ** (i + flag)

        return index
    def toStrg(self):
        return (str(self.get_index())+"\n"+ ("grd:" + str(self.grounded)+"\n" )+ ("boundtfdomain:"+self.boundtfdomain+"\n")+ (" root:" + str(self.root) +"\n") + ("$" if self.changeable else "") + (f"id:{self.identifier}\n" if self.identifier!=None else "")+(f"id:{self.string}\n" if self.string!=None else "")+f"numb:{self.number}\n"+ (f"op:{self.operator}\n" if (self.operator!=None) else "")+ (f"step:{self.step}\n" if (self.step!=None) else "") + ("applied\n" if (self.applied == True) else "") + f"jumps:{self.jumps}\n" + f"branch:{self.branch}\n"+ (f"advance:{self.advance}\n" if self.advance==True else "") )
    
    def get_ast_node_position_in_fig(self):
            # Filter out non-zero bits
            non_zero_bits = [bit for bit in self.jumps if bit != 0]
            
            # Calculate y position as the length of non-zero bits
            y_pos = len(non_zero_bits)*1.5
            
            # Calculate x position
            x_pos = sum(bit_value * 2 ** bit_index for bit_index, bit_value in enumerate(reversed(self.jumps)))*1.5
            
            return x_pos, y_pos
class GraphEdge:
    def __init__(self, sourceNode:GraphNode, toNode:GraphNode) -> None:
        self.sourceIndex = sourceNode.get_index()
        self.toIndex = toNode.get_index()


import networkx as nx

class ASTNetworkXGraph:
    def __init__(self, extendedGroundingState:ExtendedGroundingState, 
                 pred = "position",applied_jumps = [0]*max_ast_depth, advance:bool = 0, rpsign:float = 0, rpval:float = 0, rule:bool = False ):
        """
        Initializes the parser with a list of TAC strings.
        :param tac_list: List of TAC instructions, each as a string.
        """
        if (pred!='position' and pred!='solo' and pred != 'advance' and pred != 'reward'):
            raise ValueError(f'Prediction must be position/ advance/ reward, but is {pred}')
        self.domainstepmap = extendedGroundingState.domainstepmap
        self.pred = pred
        self.step = extendedGroundingState.step
        self.indomain = extendedGroundingState.indomain
        self.succeed = extendedGroundingState.succeed
        self.applied_jumps = applied_jumps
        self.advance = advance
        self.graph = nx.DiGraph() # Use DiGraph for directed graph
        self.tac_list = extendedGroundingState.extendedCodeTable.extendedCodeList
        self.sat_tac_posmap:PositionMapping =  extendedGroundingState.extendedCodeTable.pos_map
        self.parse_tac()
        self.rpsign = rpsign
        self.rpval = rpval
        self.rule = rule
    def setGsDomain(self, gsdomain:str=""):
        '''
        invake this function for predicting, and gs should be in consistancy with the gs of dsnn.
        '''
        self.gsdomain = gsdomain
        if (gsdomain!="" and (self.domainstepmap.get(gsdomain) is not None)):
            self.step = self.domainstepmap.get(gsdomain)
        else:
            self.step = 0


    def parse_tac(self):
        """
        Parses the TAC list and constructs a NetworkX graph.
        """
        def last_non_zero_bit(ast_position:list):
            # Reverse the list to start checking from the least significant bit
            reversed_list = ast_position[::-1]
            # Loop over the reversed list to find the last non-zero bit
            for bit in reversed_list:
                if bit != 0:
                    return bit
            return 0  # Return 0 if all bits are zero
        
        typeid = "identifier"
        typenumb = "number"
        typefun = "funcName"
        typestr = "string"
        typeother = "other"
        typecall = "call"
        typecomma = "comma"
        labels={}
        figpos = {}
        for tacpos, tac in enumerate(self.tac_list):
            # Example parsing logic, this will need to be adjusted
            # to match the specifics of your TAC format.

            op1 = tac.extendedOperand1
            op2 = tac.extendedOperand2
            opr = tac.extendedOperator
            res = tac.extendedResult
            grounded = tac.grounded
            boundtfdomain = tac.boundtfdomain
            root = tac.root
            nodelhs, noderhs, noderes = None, None, None
            noderes = GraphNode(grounded= grounded,
                                boundtfdomain= boundtfdomain,
                                root=root,
                                changeable=res.changeable,
                                type= res.className if res.isClass else None,
                                identifier = res.argName if (res.argType == typefun or res.argType == typeother or res.argType == typeid) else None,
                                string = res.argName if (res.argType == typestr) else None,
                                number= float(res.argValue) if (res.argType ==typenumb) else 0,
                                operator= opr.argName if opr.argType!="" else None,
                                step=  self.step,
                                applied= True if ((self.pred != "position" and self.pred != "solo") and self.applied_jumps == (res.positionInAST)) else False,
                                jumps= res.positionInAST,
                                branch= last_non_zero_bit(res.positionInAST),
                                advance= self.advance if (self.pred == "reward" and self.applied_jumps == (res.positionInAST)) else False,
                                )
            
            labels[noderes.get_index()] = noderes.toStrg()
            figpos[noderes.get_index()] = noderes.get_ast_node_position_in_fig()
            self.graph.add_node(node_for_adding=noderes.get_index(),**noderes.__dict__)
            if (op1.isLeafNode== True and op1.isEmpty() == False):
                nodelhs = GraphNode(grounded= grounded,
                                    boundtfdomain= boundtfdomain,
                                root=root,
                                changeable=op1.changeable,
                                type= op1.className if op1.isClass else None,
                                identifier = op1.argName if (op1.argType == typefun or op1.argType == typeother or op1.argType == typeid) else None,
                                string = op1.argName if (op1.argType == typestr) else None,
                                number= float(op1.argValue) if (op1.argType ==typenumb) else 0,
                                operator= None,
                                step=  self.step,
                                applied= True if ((self.pred != "position" and self.pred != "solo") and self.applied_jumps == (op1.positionInAST)) else False,
                                jumps= op1.positionInAST,
                                branch= last_non_zero_bit(op1.positionInAST),
                                advance= self.advance if (self.pred == "reward" and self.applied_jumps == (op1.positionInAST)) else False,
                                )
                labels[nodelhs.get_index()] = nodelhs.toStrg()
                figpos[nodelhs.get_index()] = nodelhs.get_ast_node_position_in_fig()
                self.graph.add_node(node_for_adding=nodelhs.get_index(),**nodelhs.__dict__)
            if (op2.isLeafNode== True and op2.isEmpty() == False):
                noderhs = GraphNode(grounded= grounded,
                                    boundtfdomain= boundtfdomain,
                                root=root,
                                changeable=op2.changeable,
                                type= op2.className if op2.isClass else None,
                                identifier = op2.argName if (op2.argType == typefun or op2.argType == typeother or op2.argType == typeid) else None,
                                string = op2.argName if (op2.argType == typestr) else None,
                                number= float(op2.argValue) if (op2.argType ==typenumb) else 0,
                                operator= None,
                                step=  self.step,
                                applied= True if ((self.pred != "position" and self.pred != "solo") and self.applied_jumps == (op2.positionInAST)) else False,
                                jumps= op2.positionInAST,
                                branch= last_non_zero_bit(op2.positionInAST),
                                advance= self.advance if (self.pred == "reward" and self.applied_jumps == (op2.positionInAST)) else False,
                                )
                labels[noderhs.get_index()] = noderhs.toStrg()
                figpos[noderhs.get_index()] = noderhs.get_ast_node_position_in_fig()
                self.graph.add_node(node_for_adding=noderhs.get_index(),**noderhs.__dict__)
            if (noderes == None):
                raise ValueError(f"Noderes is None!")
            else:
                if(op1.isEmpty() == False):
                    self.graph.add_edge(noderes.get_index(),GraphNode.jumps_to_index(op1.positionInAST))
                if(op2.isEmpty() == False):
                    self.graph.add_edge(noderes.get_index(),GraphNode.jumps_to_index(op2.positionInAST))

        self.labels = labels
        self.figpos = figpos

            

    def show(self):
        """
        Visualizes the constructed graph.
        """
        # Visualize the graph
        plt.figure(figsize=(12, 10))  # Set figure size
        nx.draw(self.graph, self.figpos, labels = self.labels, with_labels=True, node_size=2000, node_color="lightblue", font_size=10, font_weight="bold")
        plt.title("AST Graph Visualization")
        plt.show()

#############
# load data
#############

# Function to balance the data not indomain and indomain but false/ true
def reorganize_list(astnetworkxgraphlist):
    # Separate the elements into two lists
    true_indomain_elements = [elem for elem in astnetworkxgraphlist if elem.indomain]

    true_succeed_elements = [elem for elem in true_indomain_elements if elem.succeed]

    false_succeed_elements = [elem for elem in true_indomain_elements if not elem.succeed]

    # Ensure true_succeed_elements is at least 67% of true_indomain_elements
    min_true_succeed_count = int(0.67 * len(true_indomain_elements))
    if len(true_succeed_elements) < min_true_succeed_count and len(true_succeed_elements)!=0:
        additional_true_succeed_elements = (min_true_succeed_count - len(true_succeed_elements))
        true_succeed_elements += true_succeed_elements * (additional_true_succeed_elements // len(true_succeed_elements))
        true_succeed_elements += random.sample(true_succeed_elements, additional_true_succeed_elements % len(true_succeed_elements))

    # Ensure false_succeed_elements is at least 33% of true_indomain_elements
    min_false_succeed_count = int(0.33 * len(true_indomain_elements)) 
    if len(false_succeed_elements) < min_false_succeed_count and len(false_succeed_elements)!=0:
        additional_false_succeed_elements = (min_false_succeed_count - len(false_succeed_elements))
        false_succeed_elements += false_succeed_elements * (additional_false_succeed_elements // len(false_succeed_elements))
        false_succeed_elements += random.sample(false_succeed_elements, additional_false_succeed_elements % len(false_succeed_elements))

    # Combine the true_succeed_elements and false_succeed_elements back into true_indomain_elements
    true_indomain_elements = true_succeed_elements + false_succeed_elements

    false_indomain_elements = [elem for elem in astnetworkxgraphlist if not elem.indomain]

    # Calculate the maximum allowable number of False elements
    max_false_indomain_elements = (len(true_indomain_elements) // 2) if len(true_indomain_elements) >0 else len(false_indomain_elements) 

    # Randomly discard excess False elements if necessary
    if len(false_indomain_elements) > max_false_indomain_elements:
        false_indomain_elements = random.sample(false_indomain_elements, max_false_indomain_elements)

    # Combine the lists back together
    reorganized_list = true_indomain_elements + false_indomain_elements
    return reorganized_list

# Function to initialize GroundingState from a JSON file or a list of file
def initialize_from_file(filename,pred:str = 'position') -> list[ASTNetworkXGraph]:
    if isinstance(filename, list):
        return [groundingStateInstance for filename_ in filename for groundingStateInstance in initialize_from_file(filename_)]
    with open(filename, "r") as file:
        data = json.load(file)
    # return [GroundingState(**entry) for entry in data]
    astnetworkxgraphlist = []
    for i, entry in enumerate(data):
        #    print("iternum:"+str(i)+"\nentry:\n"+str(entry))
        groundingStateInstance = GroundingState(**entry)
        extendedGroundingStateInstance = ExtendedGroundingState(groundingStateInstance)
        astNetworkXGraph = ASTNetworkXGraph(extendedGroundingStateInstance,advance= extendedGroundingStateInstance.advance, applied_jumps= extendedGroundingStateInstance.extendedCodeTable.pos_map.get_ast_position(extendedGroundingStateInstance.position),rpsign = extendedGroundingStateInstance.rpsign, rpval= extendedGroundingStateInstance.rpval, rule=extendedGroundingStateInstance.rule, pred=pred)
        astnetworkxgraphlist.append(astNetworkXGraph)
    astnetworkxgraphlist = reorganize_list(astnetworkxgraphlist)
    return astnetworkxgraphlist
if __name__ == "__main__":
    astgraphs= initialize_from_file(filename="1__family#1.json")
    for graph in astgraphs:
        graph.show()