#include "Studio.h"
#include <fstream>
#include <sstream>

Studio::Studio(const std::string &configFilePath) {
    std::string configfilepath=configFilePath;// here we define the path to our desired data
    std::ifstream file(configfilepath);//here we save the whole text file as one string
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
            trainers.push_back(new Trainer(line[capacity_of_trainers[current_capacity]]));
            current_capacity=current_capacity+1;
            numberoftrainers=numberoftrainers-1;
        }


        if(line[0]=='#' || line[0]=='\0')// if there is a row that starts with # or and empty row
            continue;


//now we are expecting to get work options (thier id's is by order)

//IIIIIMMMMMMMPPPPPPPPPPPPPPOOOOOOOOOORRRRRRRRTTTTTTTTTTTANNNNNNNNNNTTTTTTTTTTTTT!!!!!!!!!!!!!
//i did not know how to split using more than 1 תנאי
        std::vector<std::string> array_of_strings;
        std::stringstream ss(line[0]); // Turn the string into a stream.
        std::string tok;

        while(getline(ss, tok, ',')) {
            array_of_strings.push_back(tok);

        }
        int number_of_workourts=array_of_strings.size()/3; // every 3 strings is 1 workout
//workour name , worukout type , price
// workout_options=new std::vector<workout>();

        int index=0;
        while(number_of_workourts>0)
        {
            workout_options.push_back(Workout::Workout(index,array_of_strings[index],std::stoi(array_of_strings[index+1]),array_of_strings[index+2]));
            index=index+3;
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


