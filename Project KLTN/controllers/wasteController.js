const { ObjectId } = require('mongodb');
const mongoose = require('mongoose');
const Waste = mongoose.model('Waste');

var wasteController = {

    getByAreaId: async function(id){
        const result = await Waste.find({
            area_id: id
        });
        return result;
    },

    getAll: async function(){
        const result = await Waste.find();
        return result;
    },

    updateState: async function(id, state){
        await Waste.updateOne(
            {_id: ObjectId(id)},
            {$set: {state: state}}
        );
    },
    
    updateFullness: async function(id, w){
        if(w == 0){
            await Waste.updateOne(
                {_id: ObjectId(id)},
                {$set: {fullness: 0}}
            );
        }
        else{
            const result = await Waste.findById(id);
            var preW = (result.fullness*result.capacity)/100;
            var curW = preW + w;
            if(curW > result.capacity){
                result.fullness = 100;
            }
            else{
                result.fullness = (curW/result.capacity)*100;
            }
            await Waste.updateOne(
                {_id: ObjectId(id)},
                {$set: {fullness: result.fullness}}
            );
        }
    },

    //changed for simulating
    updatePenaltyTime: async function(id, pTime, sDay){
        var now = new Date();
        now.setDate(now.getDate() + sDay);
        var curDate = now.getFullYear() + "/" + (now.getMonth()+1) + "/" + now.getDate();
        var bin = await Waste.findById(id);
        var pTimeList = bin.penalty_time;
        if(pTime != null){
            if(now.getDate() != pTime.getDate()){
                now.setDate(pTime.getDate() + 0);
            }
            var timeDiff = 0;
            if(now.getTime()-pTime.getTime()-15000 > 0){
                timeDiff = now.getTime()-pTime.getTime()-15000;
            }
            if(pTimeList.length > 0){
                if(curDate === pTimeList[pTimeList.length - 1].date){
                    pTimeList[pTimeList.length - 1].time += (timeDiff)*60/1000;
                }
                else{
                    if(pTimeList.length == 7){
                        pTimeList.splice(0, 1);
                    }
                    const newpTime = {
                        time: (timeDiff)*60/1000,
                        date: curDate
                    };
                    pTimeList.push(newpTime);
                }
            }
            else{
                const newpTime = {
                    time: (timeDiff)*60/1000,
                    date: curDate
                };
                pTimeList.push(newpTime);
            }
        }
        else{
            if(pTimeList.length > 0){
                if(curDate !== pTimeList[pTimeList.length - 1].date){
                    if(pTimeList.length == 7){
                        pTimeList.splice(0, 1);
                    }
                    const newpTime = {
                        time: 0,
                        date: curDate
                    };
                    pTimeList.push(newpTime);
                }
            }
            else{
                const newpTime = {
                    time: 0,
                    date: curDate
                };
                pTimeList.push(newpTime);
            }
        }
        await Waste.updateOne(
            {_id: id},
            {$set: {penalty_time: pTimeList}}
        );
    },

    updateDensity: async function(id, weight, sDay){
        var now = new Date();
        now.setDate(now.getDate() + sDay);
        var curDate = now.getFullYear() + "/" + (now.getMonth()+1) + "/" + now.getDate();
        var bin = await Waste.findById(id);
        var densList = bin.density;
        if(densList.length > 0){
            if(curDate === densList[densList.length - 1].date){
                densList[densList.length - 1].weight += weight;
            }
            else{
                if(densList.length == 7){
                    densList.splice(0, 1);
                }
                const newDens = {
                    weight: weight,
                    date: curDate
                };
                densList.push(newDens);
            }
        }
        else{
            const newDens = {
                weight: weight,
                date: curDate
            };
            densList.push(newDens); 
        }
        await Waste.updateOne(
            {_id: id},
            {$set: {density: densList}}
        );
    },

    checkForRouting: async function(area_id){
        var list = await this.getByAreaId(area_id);
        var result = [];
        var count = 0;
        var countPen = 0;
        var wasteDensity = 1.5;
        list.forEach(function(loc){
            if(loc.fullness != null && loc.fullness >= 70){
                count++;
                // if(loc.fullness >= 90){
                //     countPen++;
                // }
                if(!result.includes(loc)){
                    result.push(loc);
                }
            }
            else if(loc.fullness != null && loc.fullness < 70){
                var cWaste = loc.capacity*loc.fullness/100;
                if(cWaste + wasteDensity*15 >= loc.capacity*70/100){
                    if(!result.includes(loc)){
                        result.push(loc);
                    }
                }
            }
        });
        if(count/(7) >= 0.6){return result;}
        return [];
    }

};

module.exports = wasteController;