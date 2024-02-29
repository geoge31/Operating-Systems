/*  
      #######################################################
      #                                                     #
      #           Assignment 1 - hy345                      # 
      #           Giogos Geramoutsos, @csd3927              #
      #                                                     #
      #######################################################                                                           
*/

#include"shell.h"


void print_command_prompt()
{
      char cwd[1024]; 
      char *dirPath;

      dirPath = NULL;
      dirPath = (char*) malloc (sizeof(char));

      dirPath = getcwd(cwd,sizeof(cwd));
      printf("csd3927-hy345sh@%s:%s$ ",getlogin(),dirPath);
         
}



void handle_input(char* str)
{     
      char*       token;
      char**      argmts;
      int         pipes_check,redir_check,counter,i;

      pipes_check,redir_check,counter,i = 0;

      token = strtok(str,";");

      while(token)
      {
            pipes_check = hasPipes(token);
            redir_check = hasRedirections(token);

            if(pipes_check > 0)
            {
                  pipeline(token,pipes_check);
                  token = strtok(NULL,";"); 
            }

            else if(redir_check > 0)
            {                  
                  redirections(token,redir_check);
                  token = strtok(NULL,";");     
            }

            else
            {
                  if(strcmp(token,"exit") ==0)
                  {
                        exit(EXIT_SUCCESS);
                  }

                  fork_process(token);
                  token = strtok(NULL,";"); 
            }       
      }
}



void fork_process(char* cmd)
{
      pid_t       pid;
      char**      cmd_arguments;

      cmd_arguments = NULL;

         pid = fork();

         if(pid < 0)
         {
            perror("FORKING FAILURE(shell.c - fork_process()");
            exit(EXIT_FAILURE);
         }

         else if(pid == 0)
         {
            cmd_arguments = set_arguments(cmd);
            execute_process(cmd_arguments);
            free(cmd_arguments);  
         }

         else
         {
            waitpid(pid,NULL,0);
         }
}



char** set_arguments(char* cmd)
{
      char*       token;
      char**      exec_args;
      int         i;

      exec_args = (char**) malloc (sizeof(char*) * MAX_ARGS);

      i = 0;

      token = strtok(cmd," ");
      
      if(exec_args)
      {
            while(token)
            {
                  exec_args[i] = token;
                  token = strtok(NULL," ");
                  i++;
            }

            exec_args[i] = NULL;

            return exec_args;
      }

      else
      {
            return NULL;
      }   
}



void execute_process(char* exec_args[MAX_ARGS])
{
      if(strcmp(exec_args[0],"cd") == 0)
      {
           if(exec_args[1]!=NULL)
           {
                  chdir(exec_args[1]);
           }
           else
           {
            perror("Error with cd directory");
            exit(EXIT_FAILURE);
           } 
      }

      else 
      {
            if(execvp(exec_args[0],exec_args) == -1)
            {
                  perror("EXECUTION FAILURE (shell.c - execute_process())");
                  exit(EXIT_FAILURE);
            }
      }
      
      
}



int hasPipes(char *str)
{
      int pipe_counter,i;

      pipe_counter,i = 0;

      while(i < strlen(str))
      {
            if(str[i] == '|')
            {
                pipe_counter++;
            }

            i++;
      }
      return pipe_counter;
}



int hasRedirections(char* str)
{
      char* in;
      char* out;
      char* app;

      in = strstr(str,"<");
      out = strstr(str,">");
      app = strstr(str,">>");

      if(in)
      {
            return 1;
      }
      else if(out)
      {
            return 2;
      }
      else if(app)
      {
            
            return 3;
      }
      else 
      {
            return 0;
      }
}



void pipeline(char *pipesString, int totalPipes)
{
      char*       token;
      char**      cmd_arguments;
      int         pipesfd[totalPipes * 2];
      int         i,j;
      pid_t       pid;

      i,j = 0;

      token = strtok(pipesString,"|");

      for(i=0; i<totalPipes*2; i++)           
      {                                                 /* Create the pipes for the processes */
            if(pipe(pipesfd + i * 2) < 0)
            {
                  perror("PIPE FAILURE here");
                  exit(EXIT_FAILURE);
            }
      }

      for(i=0; i<totalPipes+1; i++)
      {                                               /* Fork for all the process needed */

            pid = fork();

            if(pid < 0)
            {
                  perror("FORK FAILURE (shell.c - pipeline())");
                  exit(EXIT_FAILURE);
            }

            if(pid == 0)  
            {
                  
                  cmd_arguments = set_arguments(token);
                  
                  if (i != 0)
			{
				dup2(pipesfd[(i - 1) * 2], 0);
			}

			if (i != totalPipes)
			{
				dup2(pipesfd[(i * 2) + 1], 1);
			} 

                  for (j = 0; j < 2 * totalPipes; j++)
			{
				close(pipesfd[j]);
			}

                  execute_process(cmd_arguments);
            }
            
            token = strtok( NULL,"|");
      }
    
      for(i=0; i<totalPipes*2; i++)
      {
            close(pipesfd[i]);
      }

      for(i=0; i<totalPipes+1; i++)
      {
            waitpid(pid,NULL,0);
      }     
}



void redirections(char* str, int redir_flag)
{
      char*       token;
      char*       token2;
      char*       token3;
      char**      argmnts;

      token = NULL;
      token2 = NULL;
      argmnts = NULL;

      switch(redir_flag)
      {
            case 1:

                        token = strtok(str,"<");
                        token2 = strtok(NULL,"<");
                        argmnts = set_arguments(token);
                        break;
            case 2:
                        token = strtok(str,">");
                        token2 = strtok(NULL,">");
                        argmnts = set_arguments(token);
                        break;
            case 3:     
                        token = strtok(str,">>");
                        token = strtok(NULL,">>");
                        argmnts = set_arguments(token);
                        break;
            default:
                        break;
      }
      
      token3 = removeSpaces(token2);  
      execute_redirections(argmnts,token3,redir_flag);
}



void execute_redirections(char** cmd1, char* cmd2, int redir)
{
      pid_t pid,wpid;
      int   status;
      int   original_stdout;

      pid = fork();

      if(pid < 0)
      {
            perror("FORK FAILURE (shell.c - execute_redirections())");
            exit(EXIT_FAILURE);
      }

      else if(pid == 0)
      {
            if(redir != 0)
            {
                  if(redir == 1)
                  {
                        int fd = open(cmd2, O_RDONLY); 

                        if(fd == -1)
                        {
                              perror("open");
                              exit(EXIT_FAILURE);
                        }  

                        dup2(fd, STDIN_FILENO);
                        close(fd);

                  }
                  
                  if(redir == 2)
                  {
                        int fd = open(cmd2, O_WRONLY | O_CREAT | O_TRUNC, 0666);

                        if(fd == -1)
                        {
                              perror("open");
                              exit(EXIT_FAILURE);  
                        }
                        dup2(fd,STDOUT_FILENO);
                        close(fd);
                  }
            }
           
            if(execvp(cmd1[0],cmd1) == -1 )
            {
                  perror("EXECUTION FAILURE");
                  exit(EXIT_FAILURE);
            }

      }
      else
      {
            do
		{
		      wpid = waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}

}



char* removeSpaces(char* input)
{
    int i = 0, j = 0;
    if(input == NULL){
      printf("Null\n");
    }
    while (input[i]) {
        if (input[i] != ' ') {
            input[j++] = input[i];
        }
        i++;
    }
    input[j] = '\0';

    return input;
}


