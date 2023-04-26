class Gene{

    listLocation = [];
    listTruck = [];
    timeWindows = [];
    adjacentMatrix = [];
    route = [];
    currentCapacity = [];
    fitness = Infinity;

    constructor(arrLoc, arrTruck, tWindow, adjMat){
        this.listLocation = arrLoc;
        this.listTruck = arrTruck;
        for(var i = 0; i < this.listTruck.length; i++){
            this.currentCapacity.push(arrTruck[i].currentCapacity);
        }
        this.timeWindows = tWindow;
        this.adjacentMatrix = adjMat;
    }

    getTruckByName(truckName){
        for(var i = 0; i < this.listTruck.length; i++){
            if(this.listTruck[i].name == truckName){
                return i;
            }
        }
        return -1;
    }

    getLocationByName(locName){
        for(var i = 0; i < this.listLocation.length; i++){
            if(this.listLocation[i].name == locName){
                return i;
            }
        }
        return -1;
    }

    startRoute(){
        for(var i = 0; i < this.listTruck.length; i++){
            this.route.push([this.listLocation[0], this.listTruck[i]]);
        }
    }

    endRoute(){
        for(var i = 0; i < this.listTruck.length; i++){
            this.route.push([this.listLocation[0], this.listTruck[i]]);
        }
    }

    setLocal(locName, truckName, lastLoc, curTime){
        var iLocation = this.getLocationByName(locName);
        var iTruck = this.getTruckByName(truckName);
        if(iLocation != -1 && iTruck != -1){
            var loc = this.listLocation[iLocation];
            var truck = this.listTruck[iTruck];
            var demand = loc.fullness*loc.capacity/100;
            if(this.currentCapacity[iTruck] >= demand && curTime+this.adjacentMatrix[lastLoc][iLocation] <= this.timeWindows[iLocation]){
                this.currentCapacity[iTruck] -= demand;
                this.route.push([loc, truck]);
                return true;
            }
            return false;
        }
        return false;
    }

    getLocalNotCovered(){
        var localNotCovered = [];
        for(var i = 1; i < this.listLocation.length; i++){
            var flag = false;
            for(var j = 0; j < this.route.length; j++){
                if(this.listLocation[i].name == this.route[j][0].name){
                    flag = true;
                    break;
                }
            }
            if(!flag){
                localNotCovered.push(this.listLocation[i]);
            }
        }
        return localNotCovered;
    }

    cost(){
        var moveTime = 0;
        var penTime = 0;
        var routeTruck = [];
        var workingTruck = 0;

        for(var i = 0; i < this.listTruck.length; i++){
            for(var j = 0; j < this.route.length; j++){
                if(this.route[j][1].name == this.listTruck[i].name){
                    if(routeTruck[i] == undefined){
                        routeTruck[i] = [];
                    }
                    routeTruck[i].push(this.route[j]);
                }
            }
        }

        for(var i = 0; i < this.listTruck.length; i++){
            workingTruck++;
            for(var j = 0; j < routeTruck[i].length-1; j++){
                var iLocStart = this.getLocationByName(routeTruck[i][j][0].name);
                var iLocEnd = this.getLocationByName(routeTruck[i][j+1][0].name);
                if(iLocStart != -1 && iLocEnd != -1){
                    moveTime += this.adjacentMatrix[iLocStart][iLocEnd];
                    if(iLocEnd != 0){
                        var temp = moveTime - this.timeWindows[iLocEnd];
                        if(temp > 0){
                            penTime += temp;
                        }
                    }
                }

            }
        }
        this.fitness = moveTime+penTime+0.25*workingTruck;
        return [moveTime, penTime];
    }

    crossOver(anotherGene){
        var locs = this.listLocation;
        var trucks = this.listTruck;
        var child1 = new Gene(locs, trucks, this.timeWindows, this.adjacentMatrix);
        var child2 = new Gene(locs, trucks, this.timeWindows, this.adjacentMatrix);
        child1.startRoute();
        child2.startRoute();
        var lastLoc1 = [];
        var curTime1 = [];
        var lastLoc2 = [];
        var curTime2 = [];
        for(var i = 0; i < trucks.length; i++){
            lastLoc1.push(0);
            curTime1.push(0);
            lastLoc2.push(0);
            curTime2.push(0);
        }
        for(var i = 0; i < this.route.length; i++){
            if(this.route[i][0].name != "depot"){
                var tempIndex = 0;
                for(var j = 0; j < anotherGene.route.length; j++){
                    if(anotherGene.route[j][0].name == this.route[i][0].name){
                        tempIndex = j;
                        break;
                    }
                }
                var truckIndex1 = this.getTruckByName(this.route[i][1].name);
                var truckIndex2 = this.getTruckByName(anotherGene.route[j][1].name);
                var locIndex1 = this.getLocationByName(this.route[i][0].name);
                var locIndex2 = this.getLocationByName(anotherGene.route[j][0].name);
                if(i%2 == 0){
                    if(child1.setLocal(this.route[i][0].name, this.route[i][1].name, lastLoc1[truckIndex1], curTime1[truckIndex1])){
                        curTime1[truckIndex1] += this.adjacentMatrix[lastLoc1[truckIndex1]][locIndex1];
                        lastLoc1[truckIndex1] = locIndex1;
                    }
                    if(child2.setLocal(anotherGene.route[j][0].name, anotherGene.route[j][1].name, lastLoc2[truckIndex2], curTime2[truckIndex2])){
                        curTime2[truckIndex2] += this.adjacentMatrix[lastLoc2[truckIndex2]][locIndex2];
                        lastLoc2[truckIndex2] = locIndex2;
                    }
                }
                else{
                    if(child2.setLocal(this.route[i][0].name, this.route[i][1].name, lastLoc2[truckIndex1], curTime2[truckIndex1])){
                        curTime2[truckIndex1] += this.adjacentMatrix[lastLoc2[truckIndex1]][locIndex1];
                        lastLoc2[truckIndex1] = locIndex1;
                    }
                    if(child1.setLocal(anotherGene.route[j][0].name, anotherGene.route[j][1].name, lastLoc1[truckIndex2], curTime1[truckIndex2])){
                        curTime1[truckIndex2] += this.adjacentMatrix[lastLoc1[truckIndex2]][locIndex2];
                        lastLoc1[truckIndex2] = locIndex2;
                    }
                }
            }
        }
        child1.repair(curTime1, lastLoc1);
        child2.repair(curTime2, lastLoc2);
        child1.endRoute();
        child2.endRoute();
        return [child1, child2];
    }

    mutate(mutateRate){
        for(var i = 0; i < this.route.length-3; i++){
            if(Math.random > mutateRate){
                continue;
            }
            if(this.route[i][0].name != "depot" && this.route[i+2][0].name != "depot"){
                var temp = this.route[i];
                this.route[i] = this.route[i+2];
                this.route[i+2] = temp;
            }
        }
        this.cost();
    }

    repair(timeTravel, lastLoc){
        var listNotCov = this.getLocalNotCovered();
        for(var i = 0; i < listNotCov.length; i++){
            var demand = listNotCov[i].fullness*listNotCov[i].capacity;
            var locIndex = this.getLocationByName(listNotCov[i].name);
            var iMin = 0;
            var min = timeTravel[0]+this.adjacentMatrix[lastLoc[0]][locIndex] - this.timeWindows[i];
            for(var j = 1; j < this.listTruck.length; j++){
                if(demand <= this.currentCapacity[j]){
                    var penTime = timeTravel[j]+this.adjacentMatrix[lastLoc[j]][this.getLocationByName(listNotCov[i].name)] - this.timeWindows[i];
                    if(min > penTime){
                        min = penTime;
                        iMin = j;
                    }
                }
            }
            this.route.push([listNotCov[i], this.listTruck[iMin]]);
            timeTravel[iMin] += this.adjacentMatrix[lastLoc[iMin]][locIndex];
            lastLoc[iMin] = locIndex;
        }
    }

    getRouteForEachTruck(){
        var routeTruck = [];

        for(var i = 0; i < this.listTruck.length; i++){
            for(var j = 0; j < this.route.length; j++){
                if(this.route[j][1].name == this.listTruck[i].name){
                    if(routeTruck[i] == undefined){
                        routeTruck[i] = [];
                    }
                    routeTruck[i].push(this.route[j][0]);
                }
            }
        }
        return routeTruck;
    }

}

module.exports = Gene;