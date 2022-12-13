# SoS

## Installation

```shell
git submodule update --init --recursive
```

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