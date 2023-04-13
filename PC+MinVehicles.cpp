#include<bits/stdc++.h>
using namespace std;
#define INF 99999
// Declaring input parameters globally.
int Scc = 5; // slow charge cost (charging cost in rupees / per percentage of charging)
int Scr = 10; // slow charge rate (charging % /hour)
int Fcc = 10; // Fast charge cost (charging cost in rupees / per percentage of charging)
int Fcr = 60; // Fast charge rate (charging % /hour)
int vehicles = 1; // Min Number of vehicles required
int leftOverCharge;
pair<int,int> startTime = {6,0};//Start Time of vehicle in 24-hour format.
vector<int> Path; // 1, 2, 3, 4 .. -> represent customers  -1, -2 represents charging stations
int Speed = 60; //(60 km/hour)
 /*
void printSolution(vector<vector<int>> &dist){
    int V = dist[0].size();
    printf(
            "The following matrix shows the shortest distances"
            " between every pair of vertices \n");
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (dist[i][j] == INF)
                printf("%7s", "INF");
            else
                printf("%7d", dist[i][j]);
        }
        printf("\n");
    }
}
 */
vector<int> DistExt(vector<vector<int>> &dist){
    vector<int> Distances;
    int V = dist[0].size();
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if(j == i+1){
                Distances.push_back(dist[i][j]);
            }
        }
    }
    // Distances now contain distances of adjacent nodes sorted in priority order.
    // Treating first node as depot.
    // Adding the distance between depot and last customer
    Distances.push_back(dist[5][0]);
    cout<<"Distances: "<<" ";
    for(int x: Distances){
        cout<<x<<" ";
    }
    cout<<endl;
    //PartialCharging(Distances);
    return Distances;
}
/*
 * Floyd Warshall algorithm for all pair-shortest path problem of customers.
 */
vector<int> floydWarshall(vector<vector<int>> dist){
    int i, j, k;
    //Initializing number of vertices with respective size for given graph.
    int V = dist[0].size();
    for (k = 0; k < V; k++) {
        for (i = 0; i < V; i++) {
            for (j = 0; j < V; j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j];
            }
        }
    }
    //printSolution(dist);
    return DistExt(dist);
}

int PartialCharging(int start,vector<int> &Distances,int r,vector<vector<int>> &charging,int Cost,int currentCharge){
    //start -> current node index
    // Base Case
    if(start == Distances.size()-1){
        Path.push_back(start);
        cout<<"Current Customer: "<<start<<endl;
        cout<<"Current Charge: "<<currentCharge<<endl;
        cout<<"Cost Till Now: "<<Cost<<endl;
        if(currentCharge >= r*Distances[start+1]){
            cout<<"We can Directly fo from 5 to depot"<<endl;
            leftOverCharge = currentCharge-r*Distances[start+1];
            return Cost+r*Distances[start]*Scc;
        }
        else{
            cout<<"We have to go from 5 to depot via charging station"<<endl;
            return min(r*(charging[start-1][0]+charging[0][0])*Scc,r*(charging[start-1][0]+charging[0][0]));
        }
    }
    else{
        Path.push_back(start);
        if(currentCharge < 0){
            cout<<"Not enough charge to go to charging station"<<endl;
            return Cost;
        }
        cout<<"Current Customer: "<<start<<endl;
        cout<<"Current Charge: "<<currentCharge<<endl;
        cout<<"Cost Till Now: "<<Cost<<endl;
        if(r*(Distances[start]+charging[start+1][0]) <= currentCharge || r*(Distances[start]+charging[start+1][1]) <= currentCharge){
            int CostTillNow = Cost+Distances[start]*r*Scc;
            PartialCharging(start+1,Distances,r,charging,CostTillNow,currentCharge-Distances[start]*r);
        }
        else{
            if(Distances[start]+charging[start][0] <= Distances[start]+charging[start][1]){
                Path.push_back(-1);
                cout<<"Going to charging station - 1 "<<": ";
                cout<<100-currentCharge+r*charging[start][0]<<"% filled"<<endl;
                PartialCharging(start+1,Distances,r,charging,Cost+r*charging[start][0]*Scc+r*Scc*charging[start+1][0],100-r*charging[start+1][0]);
            }
            else{
                Path.push_back(-2);
                cout<<"Going to charging station - 2"<<": ";
                cout<<100-currentCharge+r*charging[start][1]<<"% filled"<<endl;
                PartialCharging(start+1,Distances,r,charging,Cost+r*Scc*charging[start][1]+r*Scc*charging[start+1][1],100-r*charging[start+1][1]);
            }
        }
    }

}
int TimeWindow(int start,vector<int> &Distances,int r,vector<vector<int>> &charging,int Cost,int currentCharge,vector<vector<int>> & Deadlines){
    int StartTime= 6;// 6 hour 0 minutes in 24-hour format.
    vector<float> TimeToReach;
    for(int i = 0;i<Distances.size();i++){
        TimeToReach.push_back(StartTime+Distances[i]/float(Speed));
    }
    for(int x: TimeToReach){
        cout<<x<<" ";
    }


};



vector<pair<int,int>> Positions;// This Vector Holds positions of vehicles.
void MinVehicles(vector<int> &Distances, vector<vector<int>> &Deadlines,vector<int> Path,vector<vector<int>> &charging,int &CurrentTimeH,int &CurrentTimeM){
    int flag = 0;
    int index = 0;
    for(int i = 0;i<Path.size();){
        index = i;
        flag = Path[i];
        if(Path[i] >=0){
            cout<<"Currently at customer - "<<Path[i]<<endl;
            cout<<"Current time: "<<CurrentTimeH<<" "<<CurrentTimeM<<endl;
        }
        //if the next visiting station is customer else it is charging station
        if(Path[i+1] >= 0 and Path[i] >= 0){
            CurrentTimeH += Distances[Path[i]]/60;
            CurrentTimeM += Distances[Path[i]]%60;
            if(CurrentTimeM >= 60){
                CurrentTimeM -= 60;
                CurrentTimeH += 1;
            }
            // If Deadline is Not Met
            if(Deadlines[Path[i+1]][0]*60+Deadlines[Path[i+1]][1] <  CurrentTimeM+CurrentTimeH*60){
                //Code Block for reusing Existing Vehicles - Start
                if(vehicles > 1){
                    for(auto &it: Positions){
                        if(Deadlines[Path[i+1]][0]*60+Deadlines[Path[i+1]][1] > it.second){
                            it.first = Path[i+1];
                            it.second += Distances[Path[i]]*60;
                            cout<<"We can use existing vehicle"<<endl;
                            break;
                        }
                    }
                }
                //Code Block for reusing Existing Vehicles - End
                else{
                    cout<<"Need One more Vehicle for going from "<<Path[i]<<" to "<<Path[i+1]<<endl;
                    vehicles += 1;
                    //Position and Time Of New Vehicle
                    Positions.push_back({Path[i],CurrentTimeM+CurrentTimeH*60});
                }
            }
            i += 1;
        }
        else{
            if(Path[i+1] == -1){
                int d = charging[Path[i]][0]+charging[Path[i+2]][0];
                CurrentTimeH += d/60;
                CurrentTimeM += d%60;
                if(CurrentTimeM >= 60){
                    CurrentTimeM -= 60;
                    CurrentTimeH += 1;
                }
                if(Deadlines[Path[i]][0]*60+Deadlines[Path[i+2]][1] < CurrentTimeM+CurrentTimeH*60){
                    if(vehicles > 1){
                        for(auto &it: Positions){
                            if(Deadlines[Path[i]][0]*60+Deadlines[Path[i+2]][1] > it.second){
                                it.first = Path[i+1];
                                it.second += Distances[i]*60;
                                cout<<"We can use existing vehicle"<<endl;
                                break;
                            }
                        }
                    }
                    else{
                        cout<<"Need One more Vehicle for going from "<<Path[i]<<" to "<<Path[i+2]<<endl;
                        vehicles += 1;
                    }
                }
            }
            if(Path[i+1] == -2){
                int d = charging[Path[i]][1]+charging[Path[i+2]][1];
                CurrentTimeH += d/60;
                CurrentTimeM += d%60;
                if(CurrentTimeM >= 60){
                    CurrentTimeM -= 60;
                    CurrentTimeH += 1;
                }
                if(Deadlines[Path[i]][0]*60+Deadlines[Path[i+2]][1] < CurrentTimeM+CurrentTimeH*60){
                    if(vehicles > 1){
                        for(auto it: Positions){
                            if(Deadlines[Path[i]][0]*60+Deadlines[Path[i+2]][1] > it.second){
                                it.first = Path[i+1];
                                it.second += Distances[Path[i]]*60;
                                cout<<"We can use existing vehicle"<<endl;
                                break;
                            }
                        }
                    }
                    else{
                        cout<<"Need One more Vehicle for going from "<<Path[i]<<" to "<<Path[i+2]<<endl;
                        vehicles += 1;
                    }
                }
            }
            i += 2;
        }
    }

};


int main(){
    // Input:
    /* iniCharge : initial charge
     * c : Customers
     * Cs : charging stations
     * ChargingDist : distance of customers from charging stations
     * CustomerDist : distance of other customers w.r.t one
     * r : rate of discharge ( r = 1 -> 1% of charge per km )
     * Cc : Charging Cost
     *
     * Problems to be solved:
     * 1. printing Cost required for round trip.
     * 2. Charging to filled at each charging station (following partial charging)
     * 3. Minimum Number of vehicles needed for successful delivery within time.
     * Parameters needed to be programmed:
         * 1. Successful delivery considering that there is a deadline for each customer
         * 2. Considering two different charging types possible at a charging station :
         *      1. Slow charging with less price
         *      2. Fast charging with more price.
         *      slow charge:
         *          rate of charge: 20% per hour.
         *          cost: 5 rupees for 1% charging.
         *      fast charge:
         *          rate of charge: 60% per hour.
         *          cost: 10 rupees for 1% charging.
         *
     * info:
     *      1. all the vehicles will start from the depot at 8 AM.
     *      2. vehicles follow a avg speed of 60km/hour.
     *
     */

    // 1st Part - Start
    cout<<"***** 1. Start of finding path followed following partial charging *****"<<endl;
    int iniCharge = 100;
    vector<vector<int>> ChargingDist;
    std::ifstream myfile ("C:/Users/chana/CLionProjects/Inputs/1/ChargingDistances.txt");
    string ans = "";
    if(myfile.is_open()){
        char mychar;
        while(myfile){
            mychar = myfile.get();
            if(isdigit(mychar)){
                ans += mychar-'0';
            }
            //std::cout << mychar;
        }
    }
    cout<<endl;
    for(int i = 0;i<ans.size();i+=4){
        ChargingDist.push_back({int(ans[i])*10+int(ans[i+1]),int(ans[i+2])*10+int(ans[i+3])});
    }
    /*
    vector<vector<int>> ChargingDist = {{10,20},
                                        {10,30},
                                        {20,30},
                                        {60,40},
                                        {40,20},
                                        {40,30}};
    */
    for(auto it :ChargingDist){
        vector<int> v  = it;
        cout<<v[0]<<" "<<v[1]<<endl;
    }
    vector<vector<int>> CustomerDist;
    std::ifstream myfile1 ("C:/Users/chana/CLionProjects/Inputs/1/CustomerDistances.txt");
    string ans1 = "";
    vector<int> val;
    if(myfile1.is_open()){
        char mychar;
        while(myfile1){
            mychar = myfile1.get();
            if(isdigit(mychar)){
                ans1 += mychar-'0';
            }
            //cout<<mychar;
            if(mychar == ' '){
                val.push_back(int(ans1[0])*10+ans1[1]);
                ans1 = "";
            }
        }
    }
    for(int i = 0;i<6;i++){
        vector<int> V;
        for(int j = 0;j<6;j++){
            V.push_back(val[i*6+j]);
        }
        CustomerDist.push_back(V);
    }
    for(auto& it: CustomerDist){
        for(int x: it){
            cout<<x<<" ";
        }
        cout<<endl;
    }
    int r = 1; //r : rate of discharge ( r = 1 -> 1% of charge per km )
    vector<int> Distances = floydWarshall(CustomerDist);
    int Cost = 0;
    int Answer = PartialCharging(0,Distances,r,ChargingDist,Cost,iniCharge);
    cout<<"Left Over Charge: "<<leftOverCharge<<endl;
    cout<<"Overall Cost Following Partial Charging: "<<Answer-100*5<<endl;
    cout<<"***** End of finding path following partial charging *****"<<endl;
    // 1st Part - End


    // 2nd Part- Start
    // setting deadlines for customers
    // 0th index refers to deadline
    // deadline at depot is considered as starting time of vehicles
    vector<vector<int>> Deadlines = {{9,0},{10,30},{10,45},{11,00},{11,30},{12,00}}; //{10,30} represents 10 hours 30 minutes
    // time is to be taken input in 24-hour format.
    cout<<"***** 2. Start of Finding Minimum Number of vehicles required *****"<<endl;
    //Printing Path
    cout<<"Path: ";
    Path.push_back(0);
    for(int x: Path){
        cout<<x<<" ";
    }
    cout<<endl;
    cout<<"Distances: ";
    for(int x: Distances){
        cout<<x<<" ";
    }
    cout<<endl;
    int CurrentTimeH = 9;//9 hour 0 minutes is the current time when the vehicle starts
    int CurrentTimeM = 0;
    MinVehicles(Distances,Deadlines,Path,ChargingDist,CurrentTimeH,CurrentTimeM);
    cout<<"Min Number of vehicles required: "<<vehicles<<endl;
    cout<<"***** End of Finding Minimum Number of vehicles required *****"<<endl;
    cout<<"Positions of vehicles: "<<endl;
    for(auto it: Positions){
        cout<<it.first<<" "<<int(it.second/60)<<" "<<it.second%60<<endl;
    }
    // 2nd Part- End


    // 3rd Part - Start
    //cout<<"***** 3. Start of Introducing fast Charging for each customer *****"<<endl;
    //TimeWindow(0,Distances,r,ChargingDist,Cost,iniCharge,Deadlines);
    //cout<<"***** End of Introducing Fast Charging to each customer *****"<<endl;
    // 3rd Part - End



}
