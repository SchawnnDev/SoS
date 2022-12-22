# SoS
## Installation

Dependencies:
- Git
- CMake & Make
- Flex
- Bison

Test dependencies:
- Java
- Mars IDE

You can install all those dependencies & build the project using the following script :

If you're missing some packages listed above, please execute the following script as root.

```shell
./install.sh
```

Submodules will be cloned to lib/ directory

Mars will be downloaded to lib/ directory.

SoS binary will be build into cmake-build-debug/ folder.

## Usage

If you used install.sh script, please enter the cmake-build-debug/ folder.
```shell
cd cmake-build-debug/
```

To start the program, use:
```shell
./sos <args>
```

To show the help, use:
```shell
./sos --help
```


### Run

To compile a file, please give it as an argument, 
if no output file is given, it will print MIPS on stdout.

```shell
./sos <file> [-o=<file_path>]
```



## Code style

## Branches

Create a branch linked to issue.

```shell
git checkout -B <issue_id>-<short_description>
git push origin <issue_id>-<short_description>
```

Then open a merge request.

When no issue is linked: write a branch with: `hotfix-<short_description>`

## Log management
The logs are classified from the most detailed to the most discreet. When calling the compiler, you can choose the level of display of the logs.

On the development side, the different log levels should follow the following rules:

```c
log_trace(const char *message, ...);
```
`Log_trace` is used to follow the code flow, example :  `nameFunction(parameter list)`.

```c
log_debug(const char *message, ...);
```
`log_debug` is used to display values useful to follow the code, example : `nameVariable : value` 

```c
log_info(const char *message, ...);
```
`log_info` is used to monitor the condition of structures, for example: `nameStruct : nameVariable : position : numberElement`.

```c
log_warn(const char *message, ...);
```
`log_warn` is used to display information related to a non-blocking degraded procedure, example : `nomStruct : max depaced size`

```c
log_error(const char *message, ...);
```
`log_error` is used to display additional information when an error occurs in the process,
example : error value does not match expectations `variableName : value : expectedvalue`

```c
log_fatal(const char *message, ...);
```
`log_fatal` is used if `log_error` isn't enough. 