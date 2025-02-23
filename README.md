## Project Structure

This project follows a specific directory structure to organize the code and build files efficiently.

### Directories

- **ROOT**: The root directory of the file structure, located at `~/loft/C`.
- **BIN**: The bin directory is a subdirectory of `$(ROOT)`, located at `($ROOT)/bin`.
- **TRUNK**: The project directory is a subdirectory of `$(ROOT)`, located at `$(ROOT)/$(TRUNK)`.

### Build and Makefile
- The build script and Makefile are located in the `$(ROOT)` directory.
- The project being worked on is referred to as `$(TRUNK)`.
- This terminology aligns with GitHub's 'branch' terminology.
- The full build of `$(TRUNK)` is placed into `$(BIN)/<project_name>`
- Using `create <project_name>` will generate the project's file structure.
- Using `build <project_name>` will compile and run the project.

### Directory Contents

#### $(TRUNK)/build/
- `*.o`: Object files generated during the build process.

#### $(TRUNK)/include/
- `*.h`: Header files containing declarations and macro definitions.

#### $(TRUNK)/src/
- `*.c`: C source files containing the implementation code.
