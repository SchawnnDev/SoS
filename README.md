# SoS

## Installation

Please execute the following script as root:
```shell
./install.sh
```

SoS binary will be build into cmake-build-debug/ folder.

### ...

### Run
```shell
make
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