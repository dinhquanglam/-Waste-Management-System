var Gene = require('./Gene');
class Population{

    arrLocation = [];
    arrTruck = [];
    penRec = [];
    depotLocation = {
        name: "depot",
        latitude: 0,
        longitude: 0
    };
    timeWindows = [];
    adjacentMatrix = [];
    member = [];
    sizeOfPopulation = 0;
    generationNumber = 0;

    constructor(listLoc, listTruck, depot, size, genNum, pRec){
        this.depotLocation.latitude = depot.latitude;
        this.depotLocation.longitude = depot.longitude;
        this.arrLocation.push(this.depotLocation);
        this.arrLocation = this.arrLocation.concat(listLoc);
        this.arrTruck = listTruck;
        this.sizeOfPopulation = size;
        this.generationNumber = genNum;
        this.penRec = pRec;
    }

    static EuclideanDistance(start, dest){
        if ((start.latitude == dest.latitude) && (start.longitude == dest.longitude)) {
            return 0;
        }
        else {
            var radlat1 = Math.PI * start.latitude/180;
            var radlat2 = Math.PI * dest.latitude/180;
            var theta = start.longitude-dest.longitude;
            var radtheta = Math.PI * theta/180;
            var dist = Math.sin(radlat1) * Math.sin(radlat2) + Math.cos(radlat1) * Math.cos(radlat2) * Math.cos(radtheta);
            if (dist > 1) {
                dist = 1;
            }
            dist = Math.acos(dist);
            dist = dist * 180/Math.PI;
            dist = dist * 60 * 1.1515;
            dist = dist * 1.609344
            return dist;
        }
    }

    setAdjacentMatrix(){
        for(var i = 0; i < this.arrLocation.length; i++){
            var tempArr = [];
            for(var j = 0; j < this.arrLocation.length; j++){
                tempArr.push(Population.EuclideanDistance(this.arrLocation[i], this.arrLocation[j])/60);
            }
            this.adjacentMatrix.push(tempArr);
        }
    }

    setTimeWindow(wasteDensity){
        this.timeWindows.push(Infinity);
        for(var i = 1; i < this.arrLocation.length; i++){
            var index = this.penRec.findIndex(e => e.bin_id === this.arrLocation[i]._id.toString());
            if(index != -1){
                this.timeWindows.push(15);
            }
            else{
                var diffCap = (90 - this.arrLocation[i].fullness)*this.arrLocation[i].capacity/100;
                var diffTime = diffCap/wasteDensity;
                this.timeWindows.push(diffTime+0.25);
            }
        }
    }

    sort(){
        this.member.sort(function(a, b){
            return a.fitness - b.fitness;
        });
    }

    initPopulation(){
        while(this.member.length < this.sizeOfPopulation){
            var locals = this.arrLocation;
            var trucks = this.arrTruck;
            var r = new Gene(locals, trucks, this.timeWindows, this.adjacentMatrix);
            r.startRoute();
            var lastLoc = [];
            var curTime = [];
            for(var i = 0; i < trucks.length; i++){
                lastLoc.push(0);
                curTime.push(0);
            }
            var count = 0;
            for(var i = 1; i < locals.length; i++){
                var indexRandom = parseInt((Math.random()*(trucks.length)), 0);
                if(!r.setLocal(locals[i].name, trucks[indexRandom].name, lastLoc[indexRandom], curTime[indexRandom])){
                    count++;
                    if(count == 4){
                        count = 0;
                        continue;
                    }
                    i -= 1;
                }
                else{
                    curTime[indexRandom] += this.adjacentMatrix[lastLoc[indexRandom]][i]
                    lastLoc[indexRandom] = i;
                    count = 0;
                }
            }
            r.repair(curTime, lastLoc);
            r.endRoute();
            r.cost();
            this.member.push(r);
        }
        this.sort();
    }

    getBest(oldBest, newGen){
        var temp = oldBest;
        var indexBest = -1;
        for(var i = 0; i < newGen.length; i++){
            if(temp > newGen[i].fitness){
                temp = newGen[i].fitness;
                indexBest = i;
            }
        }
        return indexBest;
    }

    async generatePopulation(){
        var optimalGene = null;
        this.setAdjacentMatrix();
        this.setTimeWindow(80);
        this.initPopulation();
        var bestIndex = this.getBest(Infinity, this.member);
        var oldBest = this.member[bestIndex].fitness;
        var optimalGene = this.member[bestIndex];
        var stall = 0;
        var countGen = this.generationNumber;
        while(countGen > 0 && stall < this.generationNumber*0.3){
            var tempPop = [];
            for(var i = 0; i < this.member.length; i=i+2){
                var child = this.member[i].crossOver(this.member[i+1]);
                child[0].cost();
                child[1].cost();
                tempPop.push(this.member[i]);
                tempPop.push(this.member[i+1]);
                tempPop.push(child[0]);
                tempPop.push(child[1]);
            }
            tempPop.sort((a, b) => {return a.fitness - b.fitness});
            for(var i = 0; i < this.sizeOfPopulation; i++){
                this.member[i] = tempPop[i];
            }
            for(var g = 0; g < this.member.length; g++){
                if(Math.random > 0.8){
                    continue;
                }
                this.member[g].mutate(0.8);
            }
            bestIndex = this.getBest(oldBest, this.member);
            if(bestIndex != -1){
                if(oldBest > this.member[bestIndex].fitness){
                    optimalGene = this.member[bestIndex];
                    oldBest = optimalGene.fitness;
                    stall = 0;
                }
            }
            else{
                stall++;
            }
            countGen--;
        }
        return [oldBest, optimalGene];
    }
}

module.exports = Population;