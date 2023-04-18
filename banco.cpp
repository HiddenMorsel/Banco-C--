#include<iostream>
#include<vector>
#include<list>
#include<memory>

class Person{
public:
    std::string name;
    Person(std::string name): name(name) {}
        friend std::ostream& operator<<(std::ostream& os, const Person& p) {
        os << p.name;
        return os;
    }

};

class Bank {
    std::vector <std::shared_ptr<Person>> caixa;
    std::list <std::shared_ptr<Person>> fila_clientes;

    bool testarIndice(int indice){
        if(indice < 0 || indice >= caixa.size()){
            std::cout<<"caixa inexistente"<<"\n";
            return false;
        }
        return true;
    }

public:
    Bank(int n_caixas): caixa(n_caixas) {
        for (int i = 0; i < n_caixas; i++) {
            caixa[i] = nullptr;
        }
    }

    void entrarFila(std::shared_ptr<Person> p) {
        fila_clientes.push_back(p);
    }

    bool chamarCaixa(int indice){
        if (caixa[indice] == nullptr) {
            if (fila_clientes.empty()) {
                std::cout<<"sem clientes"<<"\n";
                return false;
            }
            caixa[indice] = fila_clientes.front();
            fila_clientes.pop_front();
            return true;
        }else if(!testarIndice(indice)){
            return false;
        }else if(caixa[indice] != nullptr){
            std::cout<<"caixa ocupada"<<"\n";
            return false;
        }
        return false;
    }

    bool sairCaixa(int indice){
        if (caixa[indice] == nullptr) {
            std::cout<<"caixa vazio"<<"\n";
            return false;
        }else if(!testarIndice(indice)){
            return false;
        } else if(caixa[indice] != nullptr){
            caixa[indice] = nullptr;
            return true;
        }
        return false;
    }

    friend std::ostream &operator<<(std::ostream &os, const Bank &b) {
        os << "Caixas: \n";
        for (int i = 0; i < b.caixa.size(); i++) {
            if (b.caixa[i] == nullptr) {
                os << "Caixa " << i <<":" << " ------" << "\n";
            } else {
                os << "Caixa " << i << ": " << *b.caixa[i] << "\n";
            }
            std::cout << "\n";
        }
        os << "Fila: ";
        if (b.fila_clientes.empty()) {
            os << "vazia" << "\n";
        } else {
            for (auto &i : b.fila_clientes) {
                os << *i << " ";
            }
            os << "\n";
        }
        return os;
    }
};

int main(){
    Bank banco(4);
    
    banco.entrarFila(std::make_shared<Person>("Antonio"));
    banco.entrarFila(std::make_shared<Person>("Maria"));
    banco.entrarFila(std::make_shared<Person>("Jose"));
    banco.entrarFila(std::make_shared<Person>("Tiago"));

    banco.chamarCaixa(0);
    banco.chamarCaixa(1);
    banco.chamarCaixa(2);
    banco.chamarCaixa(3);

    banco.chamarCaixa(0);
    banco.chamarCaixa(1);

    banco.chamarCaixa(4);

    banco.sairCaixa(0);
    banco.sairCaixa(2);
    
    std::cout<<banco <<"\n";
}