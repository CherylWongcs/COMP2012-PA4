#include "openAddressingHashTable.h"
bool check = 0;
OpenAddressingHashTable::OpenAddressingHashTable(int m, int (*hash)(string, int)):HashTable(m,(hash)){
    table = new Cell[m];
    for(int i = 0; i < m; i++){
        table[i].magic = nullptr;
        table[i].status = EMPTY;
    }
}

OpenAddressingHashTable::~OpenAddressingHashTable(){
    for(int i = 0; i < m; i++){
        delete table[i].magic;
        table[i].magic = nullptr;
    }
    delete []table;
}

void OpenAddressingHashTable::performRehashing(){
    activeCellCount = 0;
    Cell *temp = table;
    table = new Cell[2 * m];
    int temp_size = m;
    m *= 2;
    for(int i = 0; i < (m); i++){
        table[i].magic = nullptr;
        table[i].status = EMPTY;
    }

    for(int i = 0; i < temp_size ;i++){
        if(temp[i].status == ACTIVE){
            add(temp[i].magic);
        }
        
    }
    
    delete []temp;
    temp = nullptr;
}

bool OpenAddressingHashTable::add(Magic* magic){
    int active_count = 0;
    int hashvalue = hash(magic->suffix,m);

    if(HashTable::printSteps == 1){
        cout<<"hash(\""<<magic->suffix<<"\")"<<" = "<<hashvalue<<endl;
    }
    for(int i = 0; i < m ; i++){
        if(table[i].status == ACTIVE){
            active_count++;
        }
    }
    if(check) cout<<"active_count: "<<active_count<<endl;
    if(active_count > m/2){
        if(HashTable::printSteps == 1){
            cout << "Rehashing is needed!" <<endl;
        }
        performRehashing();
        hashvalue = hash(magic->suffix,m);
        if(HashTable::printSteps == 1){
            cout << "Rehashing is done!" <<endl;
        }
    }

    int R = 0 , C = 0;
    while(table[hashvalue].magic && table[hashvalue].status == ACTIVE && R < m){
        if(HashTable::printSteps == 1){
            cout<<magic->prefix + magic->suffix<<" collided at cell "<<hashvalue<<endl;
        }
        hashvalue = hi(magic->suffix, R+1);
        R++;
        
    }
    
    C = R + 1;
    if(R < m){
        comparisonCount += C;
        if(check) cout << "comparisonCount: " << comparisonCount <<endl;
        table[hashvalue].magic = magic;
        table[hashvalue].status = ACTIVE;
        activeCellCount++;
        if(HashTable::printSteps == 1){
            cout<<magic->prefix + magic->suffix<<" added at cell "<<hashvalue<<endl;
        }

        return true; 
    }
    else{
        comparisonCount += m;
        if(check) cout << " m_comparisonCount: " <<  comparisonCount <<endl;
        if(HashTable::printSteps == 1){
            cout<<magic->prefix + magic->suffix<<" cannot be added "<<endl;
        }
        return false;
    }
}

bool OpenAddressingHashTable::remove(string key){
    int hashvalue = hash(key,m);
    int R = 0 , C = 0;
    if(check) cout << "m: " <<  m <<endl;
    if(HashTable::printSteps == 1){
        cout<<"hash(\""<<key<<"\")"<<" = "<<hashvalue<<endl;
    }
    while(R < m){
        if((table[hashvalue].status == EMPTY)){
            //cout<<"HashTable::printSteps == 1: "<<(HashTable::printSteps == 1)<<endl;
            if(HashTable::printSteps == 1){
                cout<<"visited cell "<<hashvalue<<" but could not find it"<<endl;
            }
                C = R + 1;
                comparisonCount += C;
                if(check) cout << " m_comparisonCount: " <<  comparisonCount <<endl;
                if(HashTable::printSteps == 1){
                    cout<<key<<" cannot be removed "<<endl;
                }
                return false;
            
        }
        else if(table[hashvalue].status == ACTIVE) {
            
            if(table[hashvalue].magic->suffix == key){
                if(HashTable::printSteps == 1){
                    cout<<table[hashvalue].magic->prefix + table[hashvalue].magic->suffix<<" removed at cell "<<hashvalue<<endl;
                }
                table[hashvalue].status = DELETED;
                delete table[hashvalue].magic;
                table[hashvalue].magic = nullptr;
                activeCellCount--;
                C = R + 1;
                comparisonCount += C;
                if(check) cout << "comparisonCount: " << comparisonCount <<endl;
                return true; 
            }
            else{
                if(HashTable::printSteps == 1){
                    cout<<"visited cell "<<hashvalue<<" but could not find it"<<endl;
                }
                hashvalue = hi(key, R+1);
                R++;
                if(check) cout << "R: " <<  R <<endl;
            }
        }
        else{
            if(HashTable::printSteps == 1){
                cout<<"visited cell "<<hashvalue<<" but could not find it"<<endl;
            }
            hashvalue = hi(key, R+1);
            R++;
            if(check) cout << "R: " <<  R <<endl;
        }

    }
    comparisonCount += m;
    if(check) cout << " m_comparisonCount: " <<  comparisonCount <<endl;
    if(HashTable::printSteps == 1){
        cout<<key<<" cannot be removed "<<endl;
    }
    return false;
}

Magic* OpenAddressingHashTable::get(string key){
    int hashvalue = hash(key,m);
    int R = 0 , C = 0;
    if(check) cout << "m: " <<  m <<endl;
    if(HashTable::printSteps == 1){
        cout<<"hash(\""<<key<<"\")"<<" = "<<hashvalue<<endl;
    }
    while(R < m){
        if(check) cout << "hashvalue: " <<  hashvalue <<endl;
        if(check) cout << "table[hashvalue].status: " <<  table[hashvalue].status <<endl;
        if((table[hashvalue].status) == EMPTY){
            //cout<<"HashTable::printSteps == 1: "<<(HashTable::printSteps == 1);
            if(HashTable::printSteps == 1){
                cout<<"visited cell "<<hashvalue<<" but could not find it"<<endl;
            }
                C = R + 1;
                comparisonCount += C;
                if(check) cout << " m_comparisonCount: " <<  comparisonCount <<endl;
                if(HashTable::printSteps == 1){
                    cout<<key<<" cannot be found "<<endl;
                }
                return nullptr;
            
        }
        else if(table[hashvalue].status == ACTIVE){
            if(check) cout << "no empty " <<endl;
            if(table[hashvalue].magic->suffix == key){
                if(HashTable::printSteps == 1){
                    cout<<table[hashvalue].magic->prefix + table[hashvalue].magic->suffix<<" found at cell "<<hashvalue<<endl;
                }
                C = R + 1;
                comparisonCount += C;
                if(check) cout << "comparisonCount: " << comparisonCount <<endl;
                return table[hashvalue].magic; 
            }
            else{
                if(HashTable::printSteps == 1){
                    cout<<"visited cell "<<hashvalue<<" but could not find it"<<endl;
                }
                hashvalue = hi(key, R+1);
                R++;
                if(check) cout << "R: " <<  R <<endl;
            }
        }
        else{
            if(HashTable::printSteps == 1){
                cout<<"visited cell "<<hashvalue<<" but could not find it"<<endl;
            }
            hashvalue = hi(key, R+1);
            R++;
            if(check) cout << "R: " <<  R <<endl;
        }

    } 
    comparisonCount += m;
    if(check) cout << " m_comparisonCount: " <<  comparisonCount <<endl;
    if(HashTable::printSteps == 1){
        cout<<key<<" cannot be found "<<endl;
    }
    return nullptr;   
}

int OpenAddressingHashTable::getClusterCount() const{
    int count = 0, cluster = 0;
    for(int i = 0; i < m; i++){
            if(table[i].status == ACTIVE){
                if(i == m-1){ //last one
                    if(table[0].status == ACTIVE){
                        count+=2; //link with head
                    }
                    else{
                        count++;
                        cluster++;
                    }
                }
                else{
                    count++;
                }
            }
            else{
                if(count > 0){
                    cluster++;
                    count = 0;
                }
            }    
        }

    if(count > 0 && cluster == 0){
        cluster++;
    }
 
    return cluster;
}

int OpenAddressingHashTable::getLargestClusterSize() const{
    int count = 0, cluster = 0;
    int temp = 0;

    for(int i = 0; i < m; i++){
        if(table[i].status == ACTIVE){
            if(i == m-1){ //last one
                if(table[0].status == ACTIVE){
                    count+=2; //link with head
                    if(temp < count){
                        temp = count;
                    }
                }
                else{
                    count++;
                    cluster++;
                    if(temp < count){
                        temp = count;
                    }
                }
            }
            else{
                count++;
            }
        }
        else{
            if(count > 0){
                cluster++;
                if(temp < count){
                    temp = count;
                }
                count = 0;
            }
        }    
    }

    if(count > 0 && cluster == 0){
        cluster++;
        if(temp < count){
            temp = count;
        }
    }
 
    return temp;
}

string OpenAddressingHashTable::getClusterSizeSortedList() const{
    if(getClusterCount() == 0){
        return "(empty)";
    }
    int count = 0, cluster = 0, k = 0;
    //int cluster_arr[m] ;
    int *cluster_arr = new int[m];
        for(int i = 0; i < m; i++){
        if(table[i].status == ACTIVE){
            if(i == m-1){ //last one
                if(table[0].status == ACTIVE){
                    count+=2; //link with head
                    cluster_arr[0] = count;
                    if(check) cout<<"last_0_k: "<<0<<" cluster_arr[k]: "<<cluster_arr[0]<<endl;
                }
                else{
                    count++;
                    cluster++;
                    if(check) cout<<"count: "<<count<<endl;
                    cluster_arr[k] = count;
                    if(check) cout<<"last_no0_k: "<<k<<" cluster_arr[k]: "<<cluster_arr[k]<<endl;
                    k++;
                }
            }
            else{
                count++;
            }
        }
        else{
            if(count > 0){
                cluster++;
                cluster_arr[k] = count;
                if(check) cout<<"empty_k: "<<k<<" cluster_arr[k]: "<<cluster_arr[k]<<endl;;
                k++;
                count = 0;
            }
        }    
    }

    if(count > 0 && cluster == 0){
        cluster++;
        cluster_arr[k] = count;
        if(check) cout<<"allround_k: "<<k<<" cluster_arr[k]: "<<cluster_arr[k]<<endl;
        k++;
    }

    if(check) cout<<"k: "<<k<<endl;
    string list;
    
    if(check) cout<<"k*2: "<<k*2<<endl;
    int j = 0;
    for(int i = 0; i < getClusterCount(); i++){
        // cout<<"i: "<<i<<endl;
        list += std::to_string(cluster_arr[j]);
        if(check) cout<<"list: "<<list[i]<<endl;
        if(check) cout<<"cluster_arr: "<<cluster_arr[j]<<endl;
        if(check) cout<<"(k): "<<(k)<<"i < (k - 1)"<<(i < (k - 1))<<endl;
        if(i < (k - 1)){
            list += ',';
        }
        j++;  
    }
    //sorting
    for (int i = 0; i < k*2; i++)
    {
        for (int j = i + 2; j < k*2; j++)
        {
            if (list[i] < list[j])
            {
                char temp =  list[i];
                list[i] = list[j];
                list[j] = temp;
            }
            j++;
        }
        i++;
    }


    delete []cluster_arr;
    cluster_arr = nullptr;
    
    return list;   
}