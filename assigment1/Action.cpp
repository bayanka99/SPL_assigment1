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
        this->complete();

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
    this->complete();
}

std::string Order::toString() const {
    return std::string();
}

MoveCustomer::MoveCustomer(int src, int dst, int customerId):srcTrainer(id),dstTrainer(dst),id(customerId) {

}

void MoveCustomer::act(Studio &studio) {

    Trainer* trainer_soruce=studio.getTrainer(this->srcTrainer);
    Trainer* trainer_dest=studio.getTrainer(this->dstTrainer);
    if(trainer_dest== nullptr || trainer_soruce== nullptr || trainer_soruce->isOpen()== false|| trainer_dest->isOpen()==false || trainer_dest->getCustomers().size()==trainer_dest->getCapacity() || std::find(trainer_soruce->getCustomers().begin(), trainer_soruce->getCustomers().end(),this->id )!=trainer_soruce->getCustomers().end())// i am not sure if this is how to check if there is certain object in a vector
    {
        this->error("Cannot move customer");
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
        this->complete();
    }
}

std::string MoveCustomer::toString() const {
    return std::string();
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
        std::cout << "Trainer 2 closed. Salary "<<trainer->getSalary()<<" NIS"<<std::endl;
        for(auto customer=trainer->getCustomers().begin();customer!=trainer->getCustomers().end();customer++)
        {
            delete *customer;
        }
        trainer->closeTrainer();
        this->complete();

    }


}

std::string Close::toString() const {
    return std::string();
}

CloseAll::CloseAll() {

}

void CloseAll::act(Studio &studio) {
    for(int i=0;i<studio.getNumOfTrainers();i++)
    {
       if(studio.getTrainer(i)->isOpen()) {
           std::cout << "Trainer " << i << " Salary " << studio.getTrainer(i)->getSalary() << " NIS"
                     << std::endl;//if they are listed in the vector in regular order then it is ok
           studio.getTrainer(i)->closeTrainer();
           delete studio.getTrainer(i);
       }

    }
    this->complete();
}

std::string CloseAll::toString() const {
    return std::string();
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
    return std::string();
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
}

std::string PrintTrainerStatus::toString() const {
    return std::string();
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

    return std::string();
}

BackupStudio::BackupStudio() {

}

void BackupStudio::act(Studio &studio) {
    Studio* studio1=new Studio(studio);
    backup=studio1;
    this->complete();
}

std::string BackupStudio::toString() const {

    return "back up completed";
}

RestoreStudio::RestoreStudio() {

}

void RestoreStudio::act(Studio &studio) {
    if(back!= nullptr) {
        studio = *std::move(backup);
        this->complete();
    }
    else
    {
        this->error("no backup availbe");
    }

}

std::string RestoreStudio::toString() const {

    if(this->getStatus()==ERROR)
    {
        return "no backup availbe";
    }
    else
    {
        return "backup completed";
    }


}
