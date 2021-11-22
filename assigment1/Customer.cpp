#include <algorithm>
#include "Customer.h"

Customer::Customer(std::string c_name, int c_id):name(c_name),id(c_id) {

}

std::string Customer::getName() const {
    return name;
}

int Customer::getId() const {
    return id;
}

SweatyCustomer::SweatyCustomer(std::string name, int id) : Customer(name, id) {

}

std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> orderList;
    for(auto iter=workout_options.begin();iter!=workout_options.end();iter++){
        if(iter->getType()==CARDIO){
            orderList.push_back(iter->getId());
        }
    }
    return  orderList;
}

std::string SweatyCustomer::toString() const {
    std::string returnVal=getName()+",swt";
    return  returnVal;
}

Customer *SweatyCustomer::clone() const {
    return new SweatyCustomer(getName(),getId());
}

CheapCustomer::CheapCustomer(std::string name, int id) : Customer(name, id) {

}

std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> orderList;
    int minPrice=workout_options.begin()->getPrice();
    int minId=workout_options.begin()->getId();
    for(auto iter=workout_options.begin();iter!=workout_options.end();iter++){
        if(iter->getPrice()<minPrice){
            minPrice=iter->getPrice();
            minId=iter->getId();
        }
    }
    orderList.push_back(minId);
    return orderList;
}

std::string CheapCustomer::toString() const {
    std::string returnVal=getName()+", chp";
    return  returnVal;
}

Customer *CheapCustomer::clone() const{
    return new CheapCustomer(getName(),getId());
}

HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id) : Customer(name, id) {

}

std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> ordersList;
    for(auto iter=workout_options.begin();iter!=workout_options.end();iter++){
        if(iter->getType()==ANAEROBIC){
            ordersList.push_back(iter->getId());
        }
    }
    std::sort(ordersList.begin(),ordersList.end(),compareWorkouts());
    std::reverse(ordersList.begin(),ordersList.end());//check if there is a more efficient way to implement the descending order for sort
    return  ordersList;
}

std::string HeavyMuscleCustomer::toString() const {
    std::string returnVal=getName()+",mcl";
    return  returnVal;
}

Customer *HeavyMuscleCustomer::clone() const{
    return new HeavyMuscleCustomer(getName(),getId());
}

FullBodyCustomer::FullBodyCustomer(std::string name, int id) : Customer(name, id) {

}

std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<Workout> cardioList;
    std:: vector<Workout> mixedList;
    std:: vector<Workout> anaerobicList;
    for(auto iter=workout_options.begin();iter!=workout_options.end();iter++){
        if(iter->getType()==ANAEROBIC){
            anaerobicList.push_back(*iter);
        } else{
            if(iter->getType()==MIXED)
                mixedList.push_back((*iter));
            else
                cardioList.push_back(*iter);
        }
    }
    std::vector<int> ordersList;
    ordersList.push_back(std::min_element(cardioList.begin(),cardioList.end(),compareWorkouts())->getId());
    ordersList.push_back((std::max_element(mixedList.begin(),mixedList.end(),compareWorkouts())->getId()));
    ordersList.push_back(std::min_element(anaerobicList.begin(),anaerobicList.end(),compareWorkouts())->getId());
    return  ordersList;
}

std::string FullBodyCustomer::toString() const {
    std::string returnVal=getName()+",fbd";
    return  returnVal;
}

Customer *FullBodyCustomer::clone() const {
    return new FullBodyCustomer(getName(),getId());
}

bool compareWorkouts::operator()(const Workout &w1, const Workout &w2) {
    return w1.getPrice()<=w2.getPrice();
}
