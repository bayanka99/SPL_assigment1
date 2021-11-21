#include "Studio.h"
#include <fstream>
#include <sstream>


Studio::Studio(const std::string &configFilePath)
{
    std::string configfilepath=configFilePath;// here we define the path to our desired data
    std::ifstream file(configfilepath);//here we save the whole text file as one string
    char line[500];

    bool i_am_empty_or_hashtag=true;
        file.getline(line,500);
        while(i_am_empty_or_hashtag) {
            if (line[0] == '#' || line[0] == '\0')// if there is a row that starts with # or and empty row
                file.getline(line,500);
            else
            {
                i_am_empty_or_hashtag=false;
            }
        }
        //first of all we are expecting to get number of trainers
        std::string numberoftrainers=line;
         numberoftrainers.erase(remove(numberoftrainers.begin(),numberoftrainers.end(),' '));
    int number_of_trainers=std::stoi(numberoftrainers);

    i_am_empty_or_hashtag=true;

    while(i_am_empty_or_hashtag) {
        if (line[0] == '#' || line[0] == '\0')// if there is a row that starts with # or and empty row
            file.getline(line,500);
        else
        {
            i_am_empty_or_hashtag=false;
        }
    }






        //now we want to fill our trainers with thier capacity,with row 4,5,6,5... and removing ',' or ' '(space)

        //there is a chance it hase capacity of 202000
        std::string capacity_of_trainers=line;
        capacity_of_trainers.erase(remove(capacity_of_trainers.begin(),capacity_of_trainers.end(),','),capacity_of_trainers.end());
        capacity_of_trainers.erase(remove(capacity_of_trainers.begin(),capacity_of_trainers.end(),' '),capacity_of_trainers.end());


        int current_capacity=0;
        while(number_of_trainers>0)
        {
            trainers.push_back(new Trainer(capacity_of_trainers.at(current_capacity)));
            current_capacity=current_capacity+1;
            number_of_trainers=number_of_trainers-1;
        }

    i_am_empty_or_hashtag=true;

    while(i_am_empty_or_hashtag) {
        if (line[0] == '#' || line[0] == '\0')// if there is a row that starts with # or and empty row
            file.getline(line,500);
        else
        {
            i_am_empty_or_hashtag=false;
        }
    }


    //now we are expecting to get work options (thier id's is by order)

    std::string workout_options=line;
    workout_options.erase(remove(workout_options.begin(),workout_options.end(),' '),workout_options.end());// we remove every space in current line
    std::vector<std::string> array_of_strings;
    int i=0;
    while(!i_am_empty_or_hashtag)
    {
        int current_string_length=workout_options.size();
        while(i<current_string_length) {
                std::string current_word;
                 char current_char = workout_options.at(i);
                 while (current_char != ',') {
                                              current_word = current_word + current_char;
                                              i++;
                                            }
                                            array_of_strings.push_back(current_word);

                                    }
        file.getline(line,500);
        workout_options=line;
        workout_options.erase(remove(workout_options.begin(),workout_options.end(),' '),workout_options.end());// we remove every space in current line
        if (line[0] == '#' || line[0] == '\0')// if there is a row that starts with # or and empty row
            i_am_empty_or_hashtag=true;
        i=0;
    }

    int number_of_workouts=array_of_strings.size()/3;
    int current_workout_id=0;
    int current_index=0;
    while(number_of_workouts>0)
    {
        int price=std::stoi(array_of_strings.at(current_index+2));
        WorkoutType worktype;

        if(array_of_strings.at(current_index+1)=="anaerobic")
        {
            worktype=ANAEROBIC;
        }
        else
        {
            if(array_of_strings.at(current_index+1)=="mixed")
            {
                worktype=MIXED;
            }
            else
            {
                worktype=CARDIO;
            }
        }
        this->workout_options.push_back(Workout(current_workout_id,array_of_strings.at(current_index),price,worktype));
        current_index=current_index+3;
        current_workout_id=current_workout_id+1;
        number_of_workouts=number_of_workouts-1;

    }




}


void Studio::start()
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

std::vector<Workout> &Studio::getWorkoutOptions() {
    return this->workout_options;
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

BaseAction* Studio::buildaction(char * command) {
    std::string commandinstring=command;
    std::string current_command;
    int currentindex=0;
    while(commandinstring.at(currentindex)!=' ')
    {
        current_command=current_command+commandinstring.at(currentindex);
        currentindex++;
    }



    if(current_command=="workout_options")
    {
        for(auto iter=this->workout_options.begin();iter!=this->workout_options.end();iter++)
        {
            std::cout<<(*iter).Tostring()<<std::endl;
        }
    }



    workout_options
    if(current_command=="status")
    {
        std::string trainerid="";
        currentindex=currentindex+1;
        while(commandinstring.at(currentindex)!=' ')
        {
            trainerid=trainerid+commandinstring.at(currentindex);
            currentindex=currentindex+1;
        }

        PrintTrainerStatus printtrainerstatus=PrintTrainerStatus(std::stoi(trainerid));
        printtrainerstatus.act(*this);
    }


    if(current_command=="log")
    {
        PrintActionsLog print=PrintActionsLog();
        print.act(*this);
    }


    if(current_command=="backup studio")
    {
        BackupStudio backup=BackupStudio();
        backup.act(*this);
    }

    if(current_command=="restore studio")
    {
        RestoreStudio restore=RestoreStudio();
        restore.act(*this);
    }



    return nullptr;
}
