#include <iostream>
#include <sys/types.h>
#include <stdlib.h>
#include <bits/stdc++.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include  <stdio.h>
#include  <sys/types.h>
#include <signal.h>
#include <ctime>
using namespace std;

map<int,string> unclosedChildren;
/**
*
*the function parses the input line into the arguments
* it returns interger value of the number of arguments in the input
*
**/
int parseLine(string line, char* arg[])
{
    //splitting the line ..
    stringstream ss(line);
    string buffer;
    vector<string> args;
    while (ss >> buffer)
        args.push_back(buffer);
    //parsing the arguments into the arg array ..
    for(int i=0; i<args.size(); i++)
    {

        arg[i]=strdup(args[i].c_str());

    }
    arg[args.size()]='\0';

    return args.size();


}
/**
*
*the function executes the input line after it is parsed into arguments
*it takes array of arguments and the number of arguments as an input
*
**/

void  execute(char **args,int size)
{
    pid_t  pid;
    int    status;
    bool background=false;
    if(strcmp(args[size-1],"&")==0)
    {
        args[size-1]="\0";
        background=true;
    }
    if ((pid = fork()) < 0)
    {
        cout<<"cannot fork child\n";
        exit(1);
    }
    else if (pid == 0)
    {
        //executing the command
        if (execvp(*args, args) < 0)
        {
            cout<<"cannot execute command";
            exit(1);
        }
    }
    else
    {
        //check if the command will execute in background
        if(!background)
            waitpid(pid,NULL,WUNTRACED);
        else
        {
            int temp=pid;
            string a=args[0];
            unclosedChildren.insert(pair<int,string>(temp,a));

        }
    }
}

/**
*
*listens to inerrupt signal and writes the logs in the log life
*
**/
void signalHandler( int signum )
{

    ofstream myfile;
    myfile.open("log.log", std::ios_base::app);

    time_t tt;
    time( &tt );
    tm TM = *localtime( &tt );
    myfile<<(TM.tm_year+1900)<<"-"<<TM.tm_mon+2<<"-"<<TM.tm_mday;
    myfile<<" "<<TM.tm_hour<<":"<<TM.tm_min<<":"<<TM.tm_sec;
    myfile<<" INFO";
    myfile << "\t\t- Interrupt signal (" << signum << ") received "<<".\n";

}

int  main()
{
    char  *args[3];
    string temp;
    signal(SIGCHLD, signalHandler);
    signal(SIGCLD, signalHandler);


    while(1)
    {


        cout<<"Shell"<<" :& ";
        getline(cin,temp);
        cout<<"\n";
        int size=parseLine(temp,args);
        if(size==0)
        continue;
        if(strcmp(args[0],"exit")==0)
            exit(0);
        else if(strcmp(args[0],"cd")==0)
            chdir(args[1]);
        else
            execute(args,size);

    }


}
