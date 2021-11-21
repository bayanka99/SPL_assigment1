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
    this->errorMsg=errorMsg;
    status=ERROR;
    std::cout<<errorMsg<<std::endl;
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList):trainerId(id) {
    customers=customersList;
}

void OpenTrainer::act(Studio &studio) {
    studio.getTrainer(trainerId)->openTrainer();
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
    Customer* customerToAdd=studio.getTrainer(srcTrainer)->getCustomer(id);
    Trainer* trainerSrc=studio.getTrainer(srcTrainer);
    Trainer* trainerDst=studio.getTrainer(dstTrainer);
    std::vector<int> ordersToMove;
    for(auto iter=trainerSrc->getOrders().begin();iter!=trainerSrc->getOrders().end();iter++){
        if(iter->first==id)
            ordersToMove.push_back(iter->second.getId());
    }
    trainerSrc->removeCustomer(id);
    trainerDst->addCustomer(customerToAdd);
    trainerDst->order(id,ordersToMove,studio.getWorkoutOptions());
}

std::string MoveCustomer::toString() const {
    return std::string();
}

Close::Close(int id):trainerId(id) {

}

void Close::act(Studio &studio) {
    studio.getTrainer(trainerId)->closeTrainer();
    std::cout<<"Trainer "<<trainerId<<" closed. Salary "<<studio.getTrainer(trainerId)->getSalary()<<"NIS"<<std::endl;
}

std::string Close::toString() const {
    return std::string();
}

CloseAll::CloseAll() {

}

void CloseAll::act(Studio &studio) {
    for(auto iter=studio.getTrainers().begin();iter!=studio.getTrainers().end();iter++){
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
    for(auto iter=studio.getWorkoutOptions().begin();iter!=studio.getWorkoutOptions().end();iter++){
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
    backup=new Studio(studio);
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
