# Contributing to oneMKL-samples

Contributors must use forks on the main repository, to submit their changes.
All contributions must go through a pull request (PR) where they will be reviewed before being merged.

## Fork the Repository

1. To fork the repository from the GitHub user interface, click the
   **Fork** icon then select **Create a new fork**. The fork will be created
   in few seconds. If you previously forked the repo, skip to Step 3.

2. Select an **Owner** for the forked repository.

3. Click the **Create fork** button.

4. To create a branch in your fork, make sure the `main` branch is
   selected from the dropdown, and enter the name of your branch in the text field.

## Clone Your Fork

Clone the repo and checkout the branch that you just created by
entering a command similar to the following:

```
git clone -b <your branch name> https://github.com/<your GitHub username>/<your repo name>.git
```

Once you are ready to commit your changes to your repo, enter commands
similar to the following:

```
git add .
git status # to make sure only the intended files are staged; remove any extra files added by mistake
git commit -s -m "<insert commit reason here>"
git push origin
```

## Submit Pull Requests

When submitting a pull request, keep the following guidelines in mind:

- Make sure that your pull request has a clear purpose; it should be as
  simple as possible. This approach enables quicker PR reviews.

- Explain anything non-obvious from the code in comments, commit messages,
  or the PR description, as needed.

- Check the number of files being updated. Ensure that your pull request
  includes only the files you expected to be changed. (If there are
  additional files you did not expect included in the commit,
  troubleshoot before submitting the PR.)

- All pull requests must target the `main` branch.

## Log a Bug or Request a Feature

We use [GitHub Issues](https://github.com/oneapi-src/oneMKL-samples/issues)
to track sample development issues, bugs, and feature requests.

When reporting a bug, provide the following information when possible:

- Steps to reproduce the bug.
- Whether you found or reproduced the bug using the latest sample in
  the `main` branch and the latest oneMKL.
- Version numbers or other information about the CPU/GPU/device,
  platform, operating system or distribution you used to find the bug.

For usage, installation, or other requests for help, go to the
[Intel® oneAPI Forums](https://software.intel.com/en-us/forums/intel-oneapi-forums)
for more information.

## License

Code samples in this repository are licensed under the terms outlined in [License.txt](License.txt).

By contributing to the project, you agree to the license and copyright terms therein and release your contribution under these terms.
