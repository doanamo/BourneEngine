# Execute Git commands to retrieve version info from repository
execute_process(COMMAND "${GIT_EXECUTABLE}" rev-list --all --count
    OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE GIT_CHANGE_NUMBER)

execute_process(COMMAND "${GIT_EXECUTABLE}" log -1 --format=%h
    OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE GIT_COMMIT_HASH)

execute_process(COMMAND "${GIT_EXECUTABLE}" log -1 --format=%ci
    OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE GIT_COMMIT_DATE)

execute_process(COMMAND "${GIT_EXECUTABLE}" rev-parse --abbrev-ref HEAD
    OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE GIT_BRANCH_NAME)

# Generate header file with version info
configure_file(${INPUT_FILE} ${OUTPUT_FILE})
