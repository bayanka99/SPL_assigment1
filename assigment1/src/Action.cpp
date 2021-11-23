//
// Created by idanl on 17/11/2021.
//

#include "include/Action.h"
#include "include/Trainer.h"
#include "include/Studio.h"
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
        complete();
    }
}

std::string OpenTrainer::toString() const {
    std::string returnString;
    returnString+="open "+trainerId;
    returnString+=" ";
    for(auto customer:customers){
        returnString+=customer->toString();
    }
    if(getStatus()==COMPLETED)
        returnString+=" Completed";
    else
        returnString+=" Error: "+getErrorMsg();
    return returnString;
}

BaseAction *OpenTrainer::clone() const {
    std::vector<Customer*> copyOfCustomers;
    for(auto customer:customers){
        copyOfCustomers.push_back(customer->clone());
    }
    return new OpenTrainer(trainerId,copyOfCustomers);
}

Order::Order(int id) :trainerId(id){

}

void Order::act(Studio &studio) {
    Trainer* trainer=studio.getTrainer(trainerId);
    for(auto iter=*(trainer->getCustomers().begin());iter!=*(trainer->getCustomers().end());iter++){
        std::vector<int> workoutsId=iter->order(studio.getWorkoutOptions());
        trainer->order(iter->getId(),workoutsId,studio.getWorkoutOptions());
    }
    complete();
}

std::string Order::toString() const {
    std::string returnString="Order "+trainerId;
    if(getStatus()==COMPLETED){
        returnString+=" Completed";
    }
    else
        returnString+=" Error: "+getErrorMsg();
    return returnString;
}

BaseAction *Order::clone() const {
    return new Order(trainerId);
}

MoveCustomer::MoveCustomer(int src, int dst, int customerId):srcTrainer(id),dstTrainer(dst),id(customerId) {

}

void MoveCustomer::act(Studio &studio) {

    
    Trainer* trainerSrc=studio.getTrainer(srcTrainer);
    Trainer* trainerDst=studio.getTrainer(dstTrainer);
    if(trainerDst== nullptr || trainerSrc== nullptr || trainerSrc->isOpen()== false|| trainerDst->isOpen()==false || trainerDst->getCustomers().size()==trainerDst->getCapacity() || std::find(trainerSrc->getCustomers().begin(), trainerSrc->getCustomers().end(),this->id )!=trainerSrc->getCustomers().end())// i am not sure if this is how to check if there is certain object in a vector
    {
        this->error("Cannot move customer");
    }
    else{
      Customer* customerToAdd=studio.getTrainer(srcTrainer)->getCustomer(id);
      std::vector<int> ordersToMove;
      for(auto iter=trainerSrc->getOrders().begin();iter!=trainerSrc->getOrders().end();iter++){
          if(iter->first==id)
              ordersToMove.push_back(iter->second.getId());
      }
      trainerSrc->removeCustomer(id);
      trainerDst->addCustomer(customerToAdd);
      trainerDst->order(id,ordersToMove,studio.getWorkoutOptions());
      if(trainerSrc->getCustomers().size()==0)
      {
          trainerSrc->closeTrainer();
      }
      complete();
    }
}

std::string MoveCustomer::toString() const {
    std::string returnValue;
    returnValue+="move "+srcTrainer;
    returnValue+=" "+dstTrainer;
    returnValue+=" "+id;
    if(getStatus()==COMPLETED){
        returnValue+=" Completed";
    }
    else
        returnValue+=" Error: "+getErrorMsg();
    return returnValue;
}

BaseAction *MoveCustomer::clone() const {
    return new MoveCustomer(srcTrainer,dstTrainer,id);
}

Close::Close(int id):trainerId(id) {

}

void Close::act(Studio &studio) {
    Trainer* trainer=studio.getTrainer(this->trainerId);
    if(trainer== nullptr || trainer->isOpen()==false)
    {
        std::cout<<"Trainer does not exist or is not open"<<std::endl;
    }
    else {
        std::cout<<"Trainer "<<trainerId<<" closed. Salary "<<studio.getTrainer(trainerId)->getSalary()<<"NIS"<<std::endl;
        for(auto customer=trainer->getCustomers().begin();customer!=trainer->getCustomers().end();customer++)
        {
            delete *customer;
        }
        trainer->closeTrainer();
        complete();
    }
}

std::string Close::toString() const {
    std::string returnString;
    returnString+="close "+trainerId;
    if(getStatus()==COMPLETED){
        returnString+=" Completed";
    }
    else
        returnString+=" Error: "+getErrorMsg();
    return returnString;
}

BaseAction *Close::clone() const {
    return new Close(trainerId);
}

CloseAll::CloseAll() {

}

void CloseAll::act(Studio &studio) {
    for(int i=0;i<studio.getNumOfTrainers();i++)
    {
        if(studio.getTrainer(i)->isOpen()){
          std::cout << "Trainer "<<i<<" Salary "<<studio.getTrainer(i)->getSalary()<<" NIS"<<std::endl;//if they are listed in the vector in regular order then it is ok
          studio.getTrainer(i)->closeTrainer();
          delete studio.getTrainer(i);
        }
    }
    this->complete();
}

std::string CloseAll::toString() const {
    return "closeall is completed";
}

BaseAction *CloseAll::clone() const {
    return new CloseAll();
}

PrintWorkoutOptions::PrintWorkoutOptions() {

}

void PrintWorkoutOptions::act(Studio &studio) {
    std::vector<Workout> workouts=studio.getWorkoutOptions();
    for(auto workout=workouts.begin();workout!=workouts.end();workout++)
    {
        std::cout<<workout->getName()<<workout->getType()<<workout->getPrice()<<std::endl;
    }
    this->complete();
}

std::string PrintWorkoutOptions::toString() const {
    return "workout_options";
}

BaseAction *PrintWorkoutOptions::clone() const {
    return new PrintWorkoutOptions();
}

PrintTrainerStatus::PrintTrainerStatus(int id):trainerId(id) {

}

void PrintTrainerStatus::act(Studio &studio) {
    if(studio.getTrainer(this->trainerId)->isOpen()==false)
    {
        std::cout<<"Trainer "<<this->trainerId<<" status: closed"<<std::endl;
    }
    else
    {
        Trainer *trainer=studio.getTrainer(this->trainerId);
        std::cout<<"Customers: "<<std::endl;
        for(auto custom=trainer->getCustomers().begin();custom!=trainer->getCustomers().end();custom++)
        {
            std::cout<<(*custom)->getId()<<" "<<(*custom)->getName()<<std::endl;
        }

        std::cout<<"Orders: "<<std::endl;
        for(auto order=trainer->getOrders().begin();order!=trainer->getOrders().end();order++)
        {
            std::cout<<(*order).second.getName()<<" "<<(*order).second.getPrice()<<" "<<(*order).first<<std::endl;
        }
        std::cout<<"current trainer's salary: "<<trainer->getSalary()<<std::endl;
        this->complete();

    }
    complete();
}

std::string PrintTrainerStatus::toString() const {
    std::string returnString;
    returnString+="status "+trainerId;
    returnString+=" Completed";
    return returnString;
}

BaseAction *PrintTrainerStatus::clone() const {
    return new PrintTrainerStatus(trainerId);
}

PrintActionsLog::PrintActionsLog() {

}

void PrintActionsLog::act(Studio &studio) {
    std::string str;
    for(auto iter=studio.getActionsLog().begin();iter!=studio.getActionsLog().end();iter++){
        //str+=(*iter)->toString()+"\n";
        std::cout<<(*iter)->toString()<<std::endl;
    }
    this->complete();
}
std::string PrintActionsLog::toString() const {
    return "log is Completed";
}

BaseAction *PrintActionsLog::clone() const {
    return new PrintActionsLog();
}

BackupStudio::BackupStudio() {

}

void BackupStudio::act(Studio &studio) {
    Studio* studio1=new Studio(studio);
    backup=studio1;
    this->complete();
}


std::string BackupStudio::toString() const {
    return "backup completed";
}

BaseAction *BackupStudio::clone() const {
    return new BackupStudio();
}

RestoreStudio::RestoreStudio() {

}

void RestoreStudio::act(Studio &studio) {

    if(backup!= nullptr){
        studio=*std::move(backup);
        complete();
    }
    else
        error("There is no backup");
}

std::string RestoreStudio::toString() const {
    std::string returnString;
    returnString+="restore";
    if(getStatus()==COMPLETED)
        returnString+=" Completed";
    else
        returnString+=" Error: "+getErrorMsg();
    return returnString;
}

BaseAction *RestoreStudio::clone() const {
    return new RestoreStudio();
}
