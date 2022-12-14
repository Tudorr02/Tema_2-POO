#include "Deposit.h"
#include "exceptions.h"

int Deposit::number_of_products=0;

bool Deposit::search_product_by_ID(const string& _id){

    int count=0;
    string name_of_product;
    float market_price=0;
    int product_warranty=0;
    try {
        if (_id.empty() || _id.size()<7u)
            throw input_error("invalid ID in search_product_by_ID method");


        for (const auto &item: this->components) {
            if (_id == item->get_ID_product()) {
                count++;
                if (name_of_product.empty()) {
                    name_of_product = item->get_name();
                    market_price=item->get_market_price();
                    product_warranty=item->get_product_warranty();
                }
            }

        }
    } catch (input_error & e) {
        std::cout<<e.what()<<"\n";
    }


    std::cout<<"----------SEARCH---------------------------\n";
    std::cout<<"Details about product with ID ("<<_id<<") :\n";

    if(count>0){
        std::cout<<"Product name: "<<name_of_product<<"\n";
        std::cout<<"Products in deposit: "<<count<<"\n";
        std::cout<<"Price: "<<market_price<<"\n";
        std::cout<<"Warranty: "<<product_warranty<<"\n";

        std::cout<<"-------------------------------------------\n";
        return true;
    }
    else{
        std::cout<<"Product doesn't exit or it's out of stock :(\n";
        std::cout<<"-------------------------------------------\n";
        return false;
    }




}

void Deposit:: delete_product_by_ID(const string& _id, int &count) {


        if(_id.empty() || (int)_id.size()<7)
            throw input_error("Invalid ID in delete_product_by_ID method");

       // if(count<1)
           // throw input_error(" count in delete_by_ID method can't be smaller than 1");
        int position = 0;
        int total_products_with_id = 0;
        int copy_count = count;
        vector<std::shared_ptr<Component>> temporary_copy = this->components;

        for (const auto &item: temporary_copy) {
            if (_id == item->get_ID_product()) {
                total_products_with_id++;
                if (count > 0) {
                    this->components.erase(this->components.begin() + position);
                    count--;
                    number_of_products--;
                }
            }
            position++;

        }
        if(total_products_with_id==0)
           std::cout<< (" ID from the delete_product_by_ID is invalid.The product you are looking for doesn't exist in deposit");

        int difference;
        if(copy_count> total_products_with_id)
            difference=0;
        else
            difference=total_products_with_id-copy_count;

        std::cout << "Products left (in this deposit) with ID " << _id << " :" << difference << "\n";

}




void Deposit::add_product(const std::shared_ptr<Component>& item){

    this->components.push_back(item);
    number_of_products++;
    std::cout<<"\n*** Item with ID "<<item->get_ID_product()<<" has been added to deposit!\n";
}

void Deposit::add_product_by_ID(const std::string &ID) {
    for(const auto & comps: this->components ){
        if(comps->get_ID_product()==ID)
            this->add_product(comps);
    }
}

void Deposit::add_products(const vector<std::shared_ptr<Component>>& items){

        if(items.empty())
            throw input_error(" Empty vector from add_products method");
        this->components.insert(this->components.end(), items.begin(), items.end());

        for (const auto &item: items) {
            std::cout << "\n*** Item with ID " << item->get_ID_product() << " has been added to deposit!\n";
            number_of_products++;
        }

}

void Deposit:: show_products(){


        if(this->components.empty())
            throw deposit_error("Deposit is empty. Add some components before using deposit's methods!");
        for (const auto &item: this->components)
            std::cout << *item;


}

void Deposit::show_all_products(const vector<Deposit> &deposits) {
    try {
        if(deposits.empty())
            throw deposit_error("Deposit vector is empty. Add some deposit type objects before using this method!");

        std::cout << "\n-----------ALL PRODUCTS-----------\n";
        std::unordered_map<std::string, int> appearance;
        for (const auto &deposit: deposits) {

            if(deposit.components.empty())
                throw deposit_error("Deposit is empty. Add some components before using deposit's methods!");

            for (const auto &component: deposit.components) {
                if (appearance[component->get_ID_product()] == 0) {
                    appearance[component->get_ID_product()]++;
                    std::cout << "** ID: " << component->get_ID_product() << " || Name : " << component->get_name()
                              << "\n"; ////

                }

            }
        }
    } catch (deposit_error & e) {
        std::cout<<e.what()<<"\n";
    }
}

string Deposit::promo_code_() {
    std::cout<<"***PROMO CODE : ";
    if(!this->components.empty()) {
        for (const auto &component: this->components) {
            std::shared_ptr<Storage> test = std::dynamic_pointer_cast<Storage>(component);
            if (test != nullptr) {
                std::cout << test->promo_code_();
                return test->promo_code_();
            }
        }
    }
    std::cout<<"not found";
    return "not found";
}

int Deposit::get_number_of_products() {

    std::cout<<"!!!!Number of products  : "<<number_of_products<<"\n";
    return number_of_products;
}


Deposit::Deposit(const Deposit& other){

    for(const auto &item: other.components){
        this->components.push_back(item->clone());
        number_of_products++;
    }

}
Deposit& Deposit:: operator=( Deposit other){

    if(this!= &other) {
        swap((*this), other);
        number_of_products += (int) other.components.size();
    }
    return (*this);
}

void swap(Deposit& d1, Deposit& d2){
    std::swap(d1.components,d2.components);
}



///from customer
int Deposit:: number_of_orders=0;

void Deposit::market(const vector<Deposit> &deposits) {
    Deposit::show_all_products(deposits);
}

int Deposit:: get_number_of_orders(){
    return number_of_orders;
}

void Deposit:: order(const string & _id,const vector<Deposit>& deposits){


    try {
        int count = 1;
        if(_id.size()<7 || _id.empty())
            throw input_error ( "invalid ID in Customer-> order method");

        if( deposits.empty())
            throw deposit_error (" Deposits from Customer-> order method are empty. Add some deposit type object before using the method!");

        for (auto deposit: deposits) {
            std::cout << "------>IN DEPOSIT " << count++ << "\n";
            if (deposit.search_product_by_ID(_id)) {
                number_of_orders++;
                int temp=1;
                deposit.delete_product_by_ID(_id,temp);
                std::cout << "**NEW ORDER : ITEM ID :" << _id << "\nNumber of orders is now " << number_of_orders
                          << "\n";
                if(_id[0]=='S' && _id[1]=='T')
                    deposit.promo_code_();

                break;
            }
        }
    }catch(input_error& e) {
        std::cout<<e.what()<<"\n";
    }catch (deposit_error& e) {
        std::cout<<e.what()<<"\n";
    }
}