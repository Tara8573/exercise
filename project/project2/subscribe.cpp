/*Example code for pub/sub
  run as publisher
  ./pubsub pub <channelName> <message>

  run as subscriber
   ./pubsub sub <channelName>*/


/*----------------------
Publish and Subscribe
------------------------*/

#include <signal.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <hiredis/hiredis.h>
#include <hiredis/async.h>
#include <hiredis/adapters/libevent.h>
#include <unordered_map>
#include <vector>
using namespace std;

string cmd, key;

redisContext* rc = redisConnect("127.0.0.1", 6379);

/* void sub_to_map(redisAsyncContext *c, void *r, void *privdata){
	redisReply *reply = (redisReply*)r;
	if (reply == NULL){
		cout<<"Response not recev"<<endl; 
		return;
	}
	


} */

void subCallback(redisAsyncContext *c, void *r, void *privdata) {

  redisReply *reply = (redisReply*)r;
  if (reply == NULL){
    cout<<"Response not recev"<<endl; 
    return;
  }
  //cout << reply[0]->type <<" " << reply->elements<<endl;
  if(reply->type == REDIS_REPLY_ARRAY & reply->elements == 4)
  {
    if(strcmp( reply->element[0]->str,"subscribe") != 0)
    {
	  cmd = reply->element[3]->str;
	  key = reply->element[2]->str;
	  int n = key.size();
	  key = key.substr(15, n-15);
/*       cout<<"Message received -> "<<
        reply->element[3]->str<<" "<< reply->element[2]->str<<"( on channel : "<<reply->element[1]->str<<")"<<endl; */
	  cout << cmd << ' '<<key<<endl;
    }
	
	string checkcmd("hgetall ");
	checkcmd.append(key);
	redisReply *reply_check=(redisReply*)redisCommand(rc, checkcmd.c_str());
	
	int n = reply_check->elements;
	vector<string> keys;
	vector<string> values;

	for(int i=0; i<n;i=i+2){
		string _key (reply_check->element[i]->str);
		string _val (reply_check->element[i+1]->str);
		//cout << _key <<" "<< _val <<endl;
		keys.push_back(_key);
		values.push_back(_val);
	}
	//string start ("multi");
	//string end ("exec");
	//redisAppendCommand(rc, start.c_str());

	string changecmd ("select 1");
	//redisReply *reply_set=(redisReply*)redisCommand(rc, changecmd.c_str());
	redisAppendCommand(rc, changecmd.c_str());
	string strCmd;
	if(cmd == "hset"){
		strCmd = "hmset ";

		//cout << strCmd <<endl;
		strCmd += key;
    
		for(int i = 0; i < keys.size(); i++)
		{
			strCmd += " "+keys[i]+" "+values[i];
		}
		//cout << strCmd <<endl;		
	}
	else if(cmd =="del"){
		strCmd = "del ";
		strCmd += key;		
	}

    //redisReply *reply_mset=(redisReply*)redisCommand(rc, strCmd.c_str());
	redisAppendCommand(rc, strCmd.c_str());
	//redisAppendCommand(rc, end.c_str());
	
	

    for (int i = 0; i < 2;i++)
    {
        bool _status = false;
        std::string resp_str = "";
        redisReply *reply = 0;
        if(redisGetReply(rc, (void **)&reply) == REDIS_OK
                && reply != NULL
                && reply->type == REDIS_REPLY_STRING)
        {
            _status = true;
            resp_str = reply->str;
        }
        //free
        freeReplyObject(reply);
        //pipeLineReqStatus.push_back(status);
        //pipeLineReq.push_back(resp_str);
    }

  }  
  }

/* void pubCallback(redisAsyncContext *c, void *r, void *privdata) {

  redisReply *reply = (redisReply*)r;
  if (reply == NULL){
    cout<<"Response not recev"<<endl; 
    return;
  }
  cout<<"message published"<<endl;
  redisAsyncDisconnect(c);
} */

int main(int argv, char** args)
{
   string processName(args[1]);

   signal(SIGPIPE, SIG_IGN);
   struct event_base*
     base = event_base_new();

   redisAsyncContext* 
     _redisContext = redisAsyncConnect("127.0.0.1", 6379);

   if (_redisContext->err) {
     /* Let context leak for now... */
     cout<<"Error: "<< _redisContext->errstr<<endl;
     return 1;
   }

   redisLibeventAttach(_redisContext,base);
   
   int status;
  //Act as publisher
/*    if(processName.compare("pub") == 0)
   {
     string command ("publish ");
     command.append(args[2]);
     command.append (" \"");
     command.append(args[3]);
     command.append (" ");
     command.append(args[4]);
     command.append (" ");
     command.append(args[5]);
	 command.append ("\"");
	 
     status = redisAsyncCommand(_redisContext, 
           pubCallback, 
           (char*)"pub", command.c_str()); 
   } */
  //Act as subscriber
   if(processName.compare("sub") == 0)
   {
     string command ("psubscribe __key*@0__:*");
     //command.append(args[2]);

     status = redisAsyncCommand(_redisContext, 
           subCallback, 
           (char*)"sub", command.c_str()); 
/* 	 string command1 ("hgetall ");
	 command1.append(key); */
/* 	 if(command1.size()!=0){
		 status = redisAsyncCommand(_redisContext, 
			   sub_to_map, 
			   (char*)"sub", command1.c_str()); 		 
	 } */

   }
   else
     cout<<"Try pub or sub"<<endl;

  event_base_dispatch(base);
  return 0;
}