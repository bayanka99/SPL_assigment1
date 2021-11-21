#include "Studio.h"
#include <fstream>
#include <sstream>


Studio::Studio(const std::string &configFilePath):open(false),id(0)
{
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
        std::string capacity_of_trainers=line;
        capacity_of_trainers.erase(remove(capacity_of_trainers.begin(),capacity_of_trainers.end(),','),capacity_of_trainers.end());
        capacity_of_trainers.erase(remove(capacity_of_trainers.begin(),capacity_of_trainers.end(),' '),capacity_of_trainers.end());
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
    //i did not know how to split using more than 1 condition
        vector<std::string> array_of_strings;
        stringstream ss(line[0]); // Turn the string into a stream.
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
            Workout workout()
            workout_options.push_back(workout::workout(index,array_of_strings[index],std::stoi(array_of_strings[index+1]),array_of_strings[index+2]));
            index=index+3;
        }


    }


}

Studio::start()
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

BaseAction* Studio::buildAction(char* command) {
    std::string line=command;
    int index=0;
    std::string actionType="";
    while(line.at(index)!=' ')
    {
        actionType+=line.at(index);
        index++;
    }
    index++;
    if(actionType=="open"||actionType=="Open"){
        while(line.at(index)==' '){
            index++;
        }
        std::string trainerId="";
        while(line.at(index)!=' '){
            trainerId+=line.at(index);
            index++;
        }
        int id=std::stoi(trainerId);
        std::vector<Customer *> customersList;
        while(line.at(index)!='\0'){
            while(line.at(index)==' '){
                index++;
            }
            std::string  customerName="";
            while(line.at(index)!=','){
                customerName+=line.at(index);
                index++;
            }
            std::string  customerType="";
            while(line.at(index)!=' '){
                customerType+=line.at(index);
                index++;
            }
            Customer* customerToInsert;
            if(customerType._Equal("swt"))
                customerToInsert=new SweatyCustomer(customerName,id);
            if(customerType._Equal("chp"))
                customerToInsert=new CheapCustomer(customerName,id);
            if(customerType._Equal("mcl"))
                customerToInsert=new HeavyMuscleCustomer(customerName,id);
            if(customerType._Equal("fbd"))
                customerToInsert=new FullBodyCustomer(customerName,id);
            id++;
            customersList.push_back(customerToInsert);
        }
        OpenTrainer openTrainer(std::stoi(trainerId),customersList);
        openTrainer.act(*this);
    }
    if(actionType=="order"||actionType=="Order"){
        std::string trainerId="";
        while(line.at(index)!=' '){
            trainerId+=line.at(index);
            index++;
        }
        Order order(std::stoi(trainerId));
        order.act(*this);
    }
    if(actionType=="close"||actionType=="Close"){
        std::string trainerId="";
        while(line.at(index)!=' '){
            trainerId+=line.at(index);
            index++;
        }
        Close close(std::stoi(trainerId));
        close.act(*this);
    }
    if(actionType=="move"||actionType=="Move"){
        std::string srcTrainerId;
        std::string dstTrainerId;
        std::string customerId;
        while(line.at(index)!=' '){
            srcTrainerId+=line.at(index);
            index++;
        }
        while(line.at(index)==' '){
            index++;
        }
        while(line.at(index)!=' '){
            dstTrainerId+=line.at(index);
            index++;
        }
        while(line.at(index)==' '){
            index++;
        }
        while(line.at(index)!=' '){
            customerId+=line.at(index);
            index++;
        }
        MoveCustomer moveCustomer(std::stoi(srcTrainerId),std::stoi(dstTrainerId),std::stoi(customerId));
        moveCustomer.act(*this);
    }
    if(actionType=="closeall"||actionType=="CloseAll"||actionType=="Closeall"){
        CloseAll closeAll();
        closeAll().act(*this);
    }
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

Studio::Studio(const string &configFilePath) {

}
