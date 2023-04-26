const mongoose = require('mongoose');
const Car = mongoose.model('Car');

var carController = {

    getByAreaId: async function(id){
        const result = await Car.find({
            area_id: id
        });
        return result;
    },
    getByCollectorId: async function(id){
        const result = await Car.findOne({
            collector_id: id
        });
        return result;
    },

    changeCapacity: async function(car_id, capacity){
        await Car.updateOne(
            {_id: car_id},
            {$set: {capacity: capacity}}
        );
    },

    updateRoute: async function(car_id, route){
        await Car.updateOne(
            {_id: car_id},
            {$set: {routing: route}}
        );
    },

    updatePosition: async function(id, lat, lng){
        await Car.updateOne(
            {collector_id: id},
            {$set: {
                position: {
                    latitude: lat,
                    longitude: lng
                }
            }}
        );
    },

    updateCurrentCapacity: async function(id, weight){
        var car = await carController.getByCollectorId(id);
        var curCap = car.currentCapacity;
        if(weight == -1){
            curCap = 3000;
        }
        else{
            curCap -= weight;
        }
        await Car.updateOne(
            {collector_id: id},
            {$set: {currentCapacity: curCap}}
        );
    },

    //changed for simulating
    updateOpdist: async function(car_id, opDist, sDay){
        var now = new Date();
        now.setDate(now.getDate() + sDay);
        var curDate = now.getFullYear() + "/" + (now.getMonth()+1) + "/" + now.getDate();
        var car = await Car.findOne({
            _id: car_id
        });
        var opdistList = car.opDist;
        if(opdistList.length > 0){
            if(curDate === opdistList[opdistList.length - 1].date){
                opdistList[opdistList.length - 1].distance += opDist;
            }
            else{
                if(opdistList.length == 7){
                    optimeList.splice(0, 1);
                }
                const newOpdist = {
                    distance: opDist,
                    date: curDate
                };
                opdistList.push(newOpdist);
            }
        }
        else{
            const newOpdist = {
                distance: opDist,
                date: curDate
            };
            opdistList.push(newOpdist);
        }
        await Car.updateOne(
            {_id: car_id},
            {$set: {opDist: opdistList}}
        );
    },

    findAndUpdateBinFullness: async function(waste, cars){
        var emptyRoute = 0;
        for(var i = 0; i < cars.length; i++){
            var path = cars[i].routing;
            if(path.length == 0){
                emptyRoute++;
            }
            else{
                var index = path.findIndex(e => JSON.stringify(e._id) === JSON.stringify(waste._id));
                if(index != -1){
                    if(waste.fullness == 0){
                        var start = {
                            name: "position",
                            latitude: cars[i].position.latitude,
                            longitude: cars[i].position.longitude
                        };
                        path.splice(0, 1, start);
                        path.splice(index, 1);
                        await carController.updateRoute(cars[i]._id, path);
                    }
                    else{
                        if(path[index].fullness != waste.fullness){
                            path[index].fullness = waste.fullness;
                            await carController.updateRoute(cars[i]._id, path);
                        }
                    }
                    return 1;
                }
            }
        }
        if(emptyRoute == cars.length){
            return -1;
        }
        else{
            return 0;
        }
    },

    distanceBetween2Node: function(start, dest){
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
    },

    addNewBinToExistedRoute: async function(waste, cars){
        var indexTruck = null;
        var indexInsert = null;
        var minDist = Infinity;
        for(var i = 0; i < cars.length; i++){
            var currentCapacity = cars[i].currentCapacity;
            for(var j = 1; j < cars[i].routing.length-1; j++){
                var bin1 = cars[i].routing[j];
                var bin2 = cars[i].routing[j+1];
                var tempAddedDist = carController.distanceBetween2Node(bin1, waste) + carController.distanceBetween2Node(waste, bin2);
                if(tempAddedDist < minDist){
                    indexTruck = i;
                    indexInsert = j+1;
                    minDist = tempAddedDist;
                }
                currentCapacity -= bin1.fullness*bin1.capacity/100;
            }
            if(currentCapacity < waste.fullness*waste.capacity/100){
                indexTruck = null;
                indexInsert = null;
                minDist = Infinity;
            }
        }
        if(indexTruck != null && indexInsert != null){
            var path = cars[indexTruck].routing;
            path.splice(indexInsert, 0, waste);
            await carController.updateRoute(cars[indexTruck]._id, path);
        }
    }

};

module.exports = carController;