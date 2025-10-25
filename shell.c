#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>

#define TOK_DELIM " "
#define RED "\x1b[31m"
#define RESET "\x1b[0m"

char  * read_line()
{
    int buffsize = 1024;

    int position = 0 ;

    char *buffer = malloc(sizeof(char) *buffsize ) ;
    char c ;

    if (!buffer )
    {
        fprintf(stderr,"%sdash:allocation error%s\n" , RED, RESET) ;
        exit(EXIT_FAILURE) ;
    }
    while (1)
    {
        c = getchar() ;
        if(c==EOF|| c=='\n')
        {
            buffer[position] = '\0' ;
            return buffer ;
        }
        else {
            buffer[position] = c;

        }
        position ++;

        if (position >= buffsize )
        {
            buffsize +=1024 ;

            buffer = realloc(buffer, buffsize) ;
            if (!buffer )
            {
                fprintf(stderr, "dash: realloc error\n") ;
                exit(EXIT_FAILURE) ;

            }
        }



    }

}



char ** split_line(char *line )
{
    int buffsize = 1024 , position = 0  ;
    char ** tokens   = malloc(buffsize*sizeof(char *)) ;
    char *token ;

    if (tokens==NULL )
    {
        fprintf(stderr, "allocation error\n") ;
        exit(EXIT_FAILURE ) ;

    }
    token = strtok(line,TOK_DELIM) ;
    while (token !=NULL ){
        tokens[position] = token ;
        position ++;
        if(position >= buffsize)
        {
            buffsize += (1024*sizeof(char*) );
            tokens = realloc(tokens , buffsize ) ;
            if (tokens == NULL )
            {
                fprintf(stderr, "realloc error\n") ;
                exit(EXIT_FAILURE) ;
            }

        }
        token = strtok(NULL, TOK_DELIM) ;
    }
    tokens[position] =NULL;
    return tokens;
}

int dash_exit()
{
    return 0 ;
}


int dash_execute(char ** args)
{
    pid_t cpid ;
    int status ;
    if (strcmp(args[0],"exit") == 0 )
    {
        return dash_exit() ;

    }
    cpid = fork() ;

    if (cpid ==0)
    {
        if (execvp(args[0],args) < 0 )
        {
            printf("command not found: %s\n" ,args[0]);
            exit(EXIT_FAILURE);
        }

    }
    else if (cpid < 0 )
    {
        printf("error forking\n") ;
    }
    else
    {
        waitpid(cpid, &status , WUNTRACED);
    }

    return 1;
}

void loop()
{
    char *line ;
    char ** args ;

    int status = 1 ;

    do{
        printf(">");
        line = read_line();
        args = split_line(line) ;
        status = dash_execute(args) ;
        free(line) ;
        free(args);
    }while (status);
}


int main()
{
    loop() ;
    return 0 ;
}
