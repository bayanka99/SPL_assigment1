#include "Trainer.h"
#include <algorithm>
Trainer::Trainer(int t_capacity) {
    capacity=t_capacity;
    open= false;
}


int Trainer::getCapacity() const {
    return capacity;
}

void Trainer::addCustomer(Customer *customer) {
    customersList.push_back(customer);
}

void Trainer::removeCustomer(int id) {
    std::vector<Customer*>::iterator iter;
    for(iter=customersList.begin();iter!=customersList.end();iter++){
        if((*iter)->getId()==id)
            customersList.erase(iter);
    }
}

Customer *Trainer::getCustomer(int id) {
    std::vector<Customer*>::iterator iter;
    for(iter=customersList.begin();iter!=customersList.end();iter++){
        if((*iter)->getId()==id)
            return *iter;
    }
    return nullptr;//check if we need to return something else if we don't find the customers in the list
}

std::vector<Customer *> &Trainer::getCustomers() {
    return customersList;
}

std::vector<OrderPair> &Trainer::getOrders() {
    return  orderList;
}

void
Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout> &workout_options) {
    std::vector<Workout>::const_iterator iter;

    for(iter=workout_options.begin();iter!=workout_options.end();iter++){
        if(*std::find(workout_ids.begin(),workout_ids.end(),(*iter).getId())==(*iter).getId()){
            OrderPair pair= OrderPair((*iter).getId(),*iter);
            orderList.push_back(pair);
        }
    }
}

void Trainer::openTrainer() {
    open= true;
}

void Trainer::closeTrainer() {
    open= false;
}

int Trainer::getSalary() {
    return 0;
}

bool Trainer::isOpen() {
    return open;
}
