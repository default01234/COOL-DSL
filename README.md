# COOLang IDE Guide

The COOLang IDE provides a comprehensive and intuitive environment for Domain-Specific Language programming with COOL. The interface is designed for easy navigation, featuring a toolbar at the top, a project directory panel on the left, a code editor in the center, and a console output at the bottom right.
[User Interface](readmeImgs/IDE_layout.png)

## Toolbar

The toolbar contains essential tools to streamline your development experience, including basic and advanced functions:
[Toolbar](readmeImgs/IDE_toolbar.png)


### Basic Functions
- **File Management**: Open, create, and save projects or files with ease.
- **Help and Interface Settings**: Access help resources and customize the appearance of the IDE to suit your preferences.

### Advanced Functions
- **Project Settings**: Configure settings for compilation, reasoning, debugging, and AI enhancements. Updates made here will automatically modify the project settings file.
- **Compilation and Execution Control**: Start or stop the compilation and execution processes. 
- **AI Enhancement Control**: Launch or terminate AI enhancement processes. Use **Silent Mode** to suppress unnecessary AI process output. AI Enhancement cannot be run on its own. You need to activate it in the project settings and click Start during the build process.
[Compile with AI](readmeImgs/IDE_AI.png)

 
Please hover over the buttons for more details on their functionality.

##  Project Directory Panel

The project directory panel provides an organized view of all files within the current project, along with practical navigation options:
[Project Directory Panel](readmeImgs/IDE_projdir.png)


- **File Navigation**: Double-click to open files, or use the right-click menu to rename, delete, or show files in the file explorer.
- **Source Code Files**: Files are saved with the **.cos** extension (Coolang source code).
- **Main Program File**: The main program must be saved as `[project name].cos` in the project root folder.
- **Compiled Files**: The **CompiledFiles** folder contains intermediate code generated during the compilation process.

## Code Editor

The code editor offers a comfortable coding experience with:
[Code Editor](readmeImgs/IDE_codeEditor.png)

- **Syntax Highlighting**: Makes code more readable by visually distinguishing different elements of Coolang.
- **Code Hints**: Provides helpful suggestions to speed up development and reduce errors.

## Console

The scrollable console displays real-time output for user actions and code execution results. It is essential for viewing runtime behavior, debugging output, and other important messages.
[Console](readmeImgs/IDE_console.png)




---

# COOLang IDE Setup Guide

  

## Setting Up the Environment Variables

  

To fully utilize the features of Coolang IDE, including AI enhancements, it is crucial to set up the environment variables correctly. You may need to **restart your system** to enact the environment variable **'COOLANG_HOME'** for the IDE. This ensures that the IDE and its components can properly locate required resources.

  

## Python Environment Requirements for AI Enhancement

  

To apply AI enhancements in COOlang IDE, please ensure that your Python environment is set up with the following requirements:

  

1. **Python** >= 3.11.5

2. **PyTorch** (with CUDA support, if available)

3. **PyTorch Geometric**

4. **Matplotlib**

5. **NetworkX**

6. **Numpy**

7. **PyWin32**

  

These dependencies are essential for AI-related functionalities, such as advanced analysis and learning capabilities integrated into the IDE.

  

## Configuring Python Path for AI Enhancement

  

If you encounter any errors while launching the AI process, please make sure to configure the Python path correctly. This can be done by editing the **"neuralNetworkWizard.bat"** file, which is located in the following path:

  

```

software root folder/COOLANG_HOME/AI

```

  

Ensure that the Python executable path and Python home path specified in this file match your installed Python version and meets the above requirements.


*****************************************************************

For any issues or further inquiries, please refer to [coolang.org](coolang.org) for official documentation or support channels.

The COOLang IDE is a powerful tool for developing DSLs with COOLang! We will continue to update it and please contact us at coolang2022@gmail.com!