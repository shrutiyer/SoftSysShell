
void snl_loop(void)
{
  char *line;
  char **args;
  int status;

  do {
    printf("> ");
    line = snl_read_line();
    args = snl_split_line(line);
    status = snl_execute(args);

    free(line);
    free(args);
  } while (status);
}


int main(int argc, char **argv)
{
  // Load config files, if any.

  // Run command loop.
  snl_loop();

  // Perform any shutdown/cleanup.

  return EXIT_SUCCESS;
}