#include "Component.h"

#include <algorithm>
#include "exceptions.h"

Component::Component(const string &_brand,const string &_name,float _price,bool _gaming):brand(_brand),name(_name),price(_price),gaming(_gaming){

    warranty=1; ///1 an

    if(_brand.size()<2)
        throw constructor_error_invalid_attribute("brand should have at least 2 characters",_brand);

    if(_name.size()<4)
        throw constructor_error_invalid_attribute("name should have at least 4 characters",_name);

    if(_price<1)
        throw constructor_error_invalid_attribute("price can't be 0 or negative",(int)_price);


}

string Component::create_ID(const string &brand_2, const string& name_2, bool gaming_2){

    std::string new_ID;
    new_ID+= brand_2[0];
    new_ID+= brand_2[1];

    for(int i=0;i<4;i++){
        unsigned int x=(int)(name_2[i]);

        new_ID+=std::to_string((x<<1)%9);
    }

    if(gaming_2)new_ID+='G';
    else
        new_ID+="NG";

    std::transform(new_ID.begin(),new_ID.end(),new_ID.begin(),::toupper);

    return new_ID;

}

string Component::get_ID_product() {return this->ID_product;}


string Component:: get_name()const {return this->name;}
float Component::get_price() const {return this->price;}
bool Component::get_gaming_condition() const{return this->gaming;}
string Component::get_brand(){return this->brand;}
void Component::change_price(float new_price){
    if(new_price>0)
        this->price= new_price;
    else
        std::cout<<" change_price failed, negative input!\n";
}


const std::ostream &  operator<<( std::ostream& out, Component & component){

    out<<"\n-----------COMPONENT----------\n";
    out<<"->Brand : "<<component.get_brand()<<"\n";
    out<<"->Name : "<<component.name<<"\n";
    out<<"->Price : "<<component.price<<" $\n";
    std::string text;
    if(component.get_gaming_condition())
        text="true";
    else
        text="false";
    out<<"->Recommended for gaming: "<<text<<"\n";

    component.print(out);

    return out;
}