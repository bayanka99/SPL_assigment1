#include "Studio.h"
#include <fstream>
#include <sstream>


Studio::Studio(const std::string &configFilePath);
{
std::string configfilepath=configFilePath;// here we define the path to our desired data
std::std::ifstream file(configfilepath);//here we save the whole text file as one string
char line[500];
while(file)
{
file.getline(line,500);
if(line[0]=='#' || line[0]=='\0')// if there is a row that starts with # or and empty row
continue;
//first of all we are expecting to get number of trainers

int numberoftrainers=(int)line[0]-'0'; //converting number from char to int

//if(line[0]=='#' || line[0]=='\0')// if there is a row that starts with # or and empty row
// continue;

//now we want to fill our trainers with thier capacity,with row 4,5,6,5... and removing ',' or ' '(space)
std::string capacity_of_trainers=line.erase(remove(line.begin(),line.end(),',',' ,',', ',' , '),line.end());

// std::vector<Trainer*> trainers=new std::vector<Trainer*>();
int current_capacity=0;
while(numberoftrainers>0)
{
trainers.push_back(new trainer(line[capacity_of_trainers[current_capacity]]));
current_capacity=current_capacity+1;
numberoftrainers=numberoftrainers-1;
}


if(line[0]=='#' || line[0]=='\0')// if there is a row that starts with # or and empty row
continue;


//now we are expecting to get work options (thier id's is by order)

//IIIIIMMMMMMMPPPPPPPPPPPPPPOOOOOOOOOORRRRRRRRTTTTTTTTTTTANNNNNNNNNNTTTTTTTTTTTTT!!!!!!!!!!!!!
//i did not know how to split using more than 1 תנאי
vector<std::string> array_of_strings;
stringstream ss(line[0]); // Turn the string into a stream.
std::string tok;

while(getline(ss, tok, ',')) {
array_of_strings.push_back(tok);

}
int number_of_workourts=array_of_strings.length()/3; // every 3 strings is 1 workout
//workour name , worukout type , price
// workout_options=new std::vector<workout>();

int index=0;
while(number_of_workourts>0)
{
workout_options.push_back(workout::workout(index,array_of_strings[index],std::stoi(array_of_strings[index+1]),array_of_strings[index+2]));
index=index+3;
}


}


}

Studio::Start();
{

char command[500];
while(true)
{

std::cin.getline(command,500);// the input that a person types when software is running
BaseAction* act=buildaction(command);//we must implement buildaction
act->act(*this);
//this loop will stop only when there is an order close all, we decide how to do it
}
}



Trainer *Studio::getTrainer(int tid) {


    if(this->trainers.size()>tid)
    {
        return this->trainers.at(tid);
    }
    return nullptr;
}

Studio::~Studio() {

    this->workout_options.clear();
    for(auto iter=this->trainers.begin();iter!=this->trainers.end();iter++)
    {
        delete *iter;
    }
    this->trainers.clear();
    for(auto iter=this->actionsLog.begin();iter!=this->actionsLog.end();iter++)
    {
        delete *iter;
    }
    this->actionsLog.clear();

}

Studio::Studio(const Studio &other) {
    this->open=other.open;

    for(auto iter=other.trainers.begin();iter!=other.trainers.end();iter++)
    {
        this->trainers.push_back(*iter);
    }
    for(auto iter=other.actionsLog.begin();iter!=other.actionsLog.end();iter++)
    {
        this->actionsLog.push_back(*iter);
    }

    this->workout_options=other.workout_options;
}

Studio::Studio(Studio &&other) {
    this->open=other.open;

    for(auto iter=other.trainers.begin();iter!=other.trainers.end();iter++)
    {
        this->trainers.push_back(*iter);
    }
    for(auto iter=other.actionsLog.begin();iter!=other.actionsLog.end();iter++)
    {
        this->actionsLog.push_back(*iter);
    }

    this->workout_options=other.workout_options;

    other.trainers.clear();
    other.workout_options.clear();
    other.actionsLog.clear();
}

Studio &Studio::operator=(const Studio &other) {

    if(this!=&other) {
        this->workout_options.clear();
        for(auto iter=this->trainers.begin();iter!=this->trainers.end();iter++)
        {
            delete *iter;
        }
        this->trainers.clear();
        for(auto iter=this->actionsLog.begin();iter!=this->actionsLog.end();iter++)
        {
            delete *iter;
        }
        this->actionsLog.clear();

        for (auto iter = other.trainers.begin(); iter != other.trainers.end(); iter++) {
            this->trainers.push_back(*iter);
        }
        for (auto iter = other.actionsLog.begin(); iter != other.actionsLog.end(); iter++) {
            this->actionsLog.push_back(*iter);
        }

        this->workout_options = other.workout_options;
        this->open = other.open;
    }
    return *this;

}

Studio &Studio::operator=(Studio &&other) {
    if(this!=&other)
    {
        this->workout_options.clear();
        for(auto iter=this->trainers.begin();iter!=this->trainers.end();iter++)
        {
            delete *iter;
        }
        this->trainers.clear();
        for(auto iter=this->actionsLog.begin();iter!=this->actionsLog.end();iter++)
        {
            delete *iter;
        }
        this->actionsLog.clear();

        for (auto iter = other.trainers.begin(); iter != other.trainers.end(); iter++) {
            this->trainers.push_back(*iter);
        }
        for (auto iter = other.actionsLog.begin(); iter != other.actionsLog.end(); iter++) {
            this->actionsLog.push_back(*iter);
        }

        this->workout_options = other.workout_options;
        this->open = other.open;
    }
    other.workout_options.clear();
    for(auto iter=other.trainers.begin();iter!=other.trainers.end();iter++)
    {
        delete *iter;
    }
    other.trainers.clear();
    for(auto iter=other.actionsLog.begin();iter!=other.actionsLog.end();iter++)
    {
        delete *iter;
    }
    other.actionsLog.clear();

}