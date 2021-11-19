//
// Created by idanl on 17/11/2021.
//

#include "Action.h"
#include "Trainer.h"
#include "Studio.h"
BaseAction::BaseAction() {

}

ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() {
    status=COMPLETED;
}

void BaseAction::error(std::string errorMsg) {
    this->status=ERROR;
    this->errorMsg=errorMsg;
    std::cout<<"error: "<<errorMsg<<std::endl;
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList):trainerId(id) {
    customers=customersList;
}

void OpenTrainer::act(Studio &studio) {
    if(studio.getTrainer(this->trainerId)== nullptr || studio.getTrainer(this->trainerId)->isOpen()==true)
    {
        this->error("Workout session does not exist or is already open");
    }
    else
    {
        studio.getTrainer(this->trainerId)->openTrainer();
    }
}

std::string OpenTrainer::toString() const {
    return std::string();
}

Order::Order(int id) :trainerId(id){

}

void Order::act(Studio &studio) {
    Trainer* trainer=studio.getTrainer(trainerId);
    for(auto iter=*(trainer->getCustomers().begin());iter!=*(trainer->getCustomers().end());iter++){
        std::vector<int> workoutsId=iter->order(studio.getWorkoutOptions());
        trainer->order(iter->getId(),workoutsId,studio.getWorkoutOptions());
    }
}

std::string Order::toString() const {
    return std::string();
}

MoveCustomer::MoveCustomer(int src, int dst, int customerId):srcTrainer(id),dstTrainer(dst),id(customerId) {

}

void MoveCustomer::act(Studio &studio) {

    Trainer* trainer_soruce=studio.getTrainer(this->srcTrainer);
    Trainer* trainer_dest=studio.getTrainer(this->dstTrainer);
    if(trainer_dest== nullptr || trainer_soruce== nullptr || trainer_soruce->isOpen()== false|| trainer_dest.isOpen()==false || trainer_dest->getCustomers().size()==trainer_dest->getCapacity() || std::find(trainer_soruce->getCustomers().begin(), trainer_soruce->getCustomers().end(),this->id )!=trainer_soruce->getCustomers().end())// i am not sure if this is how to check if there is certain object in a vector
    {
        this->error("Cannot move customer")
    }
    else
    {
        Customer* cust=trainer_soruce->getCustomer(this->id);
        trainer_soruce->removeCustomer(this->id);
        trainer_dest->addCustomer(cust);// i think there is no need to add pairs to dest trainer
        if(trainer_soruce->getCustomers().size()==0)
        {
            trainer_soruce->closeTrainer();
        }
    }
}

std::string MoveCustomer::toString() const {
    return std::string();
}

Close::Close(int id):trainerId(id) {

}

void Close::act(Studio &studio) {
    studio.getTrainer(trainerId)->closeTrainer();
}

std::string Close::toString() const {
    return std::string();
}

CloseAll::CloseAll() {

}

void CloseAll::act(Studio &studio) {
    for(auto iter=studio.getTrainers().begin();iter!=iter=studio.getTrainers().end();iter++){
        (*iter)->closeTrainer();
        delete *iter;
    }
}

std::string CloseAll::toString() const {
    return std::string();
}

PrintWorkoutOptions::PrintWorkoutOptions() {

}

void PrintWorkoutOptions::act(Studio &studio) {
    for(auto iter=studio.getWorkoutOptions().begin();iter!=studio.getWorkoutOptions().end()){
        std::string str=iter->getName()+", ";
        str+=iter->getType()+", "+iter->getPrice();
        std::cout<<str<<std::endl;
    }
}

std::string PrintWorkoutOptions::toString() const {
    return std::string();
}

PrintTrainerStatus::PrintTrainerStatus(int id):trainerId(id) {

}

void PrintTrainerStatus::act(Studio &studio) {
    Trainer* trainer=studio.getTrainer(trainerId);
    std::string str="Trainer "+trainerId;
    str+="status "+trainer->isOpen();
    str+="\n";
    str+="Customers:";
    str+="\n";
    int index=1;
    for(auto iter=trainer->getCustomers().begin();iter!=trainer->getCustomers().end();iter++){
        str+=(*iter)->getId();
        str+=" ";
        str+=(*iter)->getName();
        str+=" \n";
    }
    str+="Orders: ";
    for(auto iter=trainer->getOrders().begin();iter!=trainer->getOrders().end();iter++){
        str+=(*iter).second.getName()+" ";
        str+=(*iter).second.getPrice()+" ";
        str+=(*iter).first;
        str+=" \n";
    }
    str+="Current Trainer’s Salary: "+trainer->getSalary();
    std::cout<<str<<std::endl;
}

std::string PrintTrainerStatus::toString() const {
    return std::string();
}

PrintActionsLog::PrintActionsLog() {

}

void PrintActionsLog::act(Studio &studio) {
    std::string str;
    for(auto iter=studio.getActionsLog().begin();iter!=studio.getActionsLog().end();iter++){
        str+=(*iter)->toString()+"\n";
    }
}

std::string PrintActionsLog::toString() const {
    return std::string();
}

BackupStudio::BackupStudio() {

}

void BackupStudio::act(Studio &studio) {
    Studio* studio1=new Studio(studio);
    backup=studio1;
}

std::string BackupStudio::toString() const {
    return std::string();
}

RestoreStudio::RestoreStudio() {

}

void RestoreStudio::act(Studio &studio) {
    studio=*std::move(backup);
}

std::string RestoreStudio::toString() const {
    return std::string();
}
