require('./models/db');

const express = require("express");
const path = require("path");
const handlebars = require("handlebars");
const exphbs = require("express-handlebars");
const {allowInsecurePrototypeAccess} = require("@handlebars/allow-prototype-access");
const bodyparser = require("body-parser");
const mongoose = require('mongoose');
var app = express();
const server = require('http').createServer(app);
const {Server} = require('socket.io');
const io = new Server(server);
const mqtt = require("mqtt");
const session = require("express-session");
const algorithm = require('./algorithm');
const algorithm2 = require('./algorithm2');
const simulate = require('./simulate');
const carTracking = require('./carTracking');
const managerRouter = require('./routers/managerRouter');
const collectorRouter = require('./routers/collectorRouter');
const accRouter = require('./routers/accountRouter');
const userRouter = require('./routers/userRouter');
var areaController = require('./controllers/areaController');
var accountController = require('./controllers/accountController');
var carController = require('./controllers/carController');
var wasteController = require('./controllers/wasteController');
var iSimulate = 0;
var Population = require('./Genetic Algorithm/Population');


app.use(bodyparser.urlencoded({extended: false}));
app.use(bodyparser.json());

app.use(express.static(path.join(__dirname, "public")));
app.use(session({
    secret: "ai201203AL104101me",
    resave: false,
    saveUninitialized: true,
    cookie: { maxAge: 60000*60*3 }
}));

io.on('connection', (socket) =>{
    socket.on('control', (msg) => {
        console.log("new message");
        var name = msg.name;
        var state = msg.state;
        console.log(name);
        console.log(state);
        var stateCtrl = "";
        if(state === "true"){
            stateCtrl = false;
        }
        else if(state === "false"){
            stateCtrl = true;
        }
        console.log(stateCtrl);
        var data = {
            state: stateCtrl
        }
        var MQTTclient = mqtt.connect('mqtt://171.244.173.204:1884');
        MQTTclient.on('connect', () =>{
            MQTTclient.publish('wasteManagement/'+name+'/control', JSON.stringify(data));
            MQTTclient.end();
        });
    });
});

app.get("/", (req, res) =>{
    if(req.session.userid){
        if(req.session.acctype == 2){
            res.redirect("http://localhost:3000/manager");
        }
        else if(req.session.acctype == 1){
            res.redirect("http://localhost:3000/collector");
        }
    }
    else{
        res.render("main_views/index", {layout: false});
    }
})

app.set('views', path.join(__dirname, '/views/'));

app.engine(
    'hbs',
    exphbs.engine({
        handlebars: allowInsecurePrototypeAccess(handlebars),
        extname: "hbs",
        defaultLayout: "mainLayout",
        layoutsDir: __dirname + "/views/layouts/",
        partialsDir: __dirname + "/views/partials/"
    })
);

app.set("view engine", "hbs");

server.listen(3000, async () =>{
    console.log("Server started at port 3000");
    // var locs = await wasteController.getAll();
    // var trucks = await mongoose.model('Car').find();
    // var area = await mongoose.model('Area').find();
    // var depot = {
    //     latitude: area[0].location[0],
    //     longitude: area[0].location[1]
    // }
    // var penRecs = await mongoose.model('Penalty_record').find();
    // var pop = new Population(locs, trucks, depot, 10, 50, penRecs);
    // var bestGene = await pop.generatePopulation();
    // //console.log(bestGene[1].route);
    // var routeTruck = bestGene[1].getRouteForEachTruck();
    // for(var i = 0; i < routeTruck.length; i++){
    //     console.log("Truck "+i+": ");
    //     console.log(routeTruck[i]);
    // }
    var dateTimer = setInterval(() => {
        iSimulate++;
        if(iSimulate == 7){
            clearInterval(dateTimer);
            console.log("Simulation ended!");
            return;
        }
        console.log(iSimulate + " day(s) ended!");
    }, 24*60000);
    //simulate.main();
    //carTracking.main();
    
});

const wasteStream = mongoose.model('Waste').watch([]);
const carStream = mongoose.model('Car').watch([]);
var messQueue = [];
var ableProcess = 0;

async function fullnessChangeProcessing(waste){
    var car = await carController.getByAreaId(waste.area_id);
    var checkUpdateFullness = await carController.findAndUpdateBinFullness(waste, car);
    if(checkUpdateFullness == 0){
        if(waste.fullness >= 70){
            await carController.addNewBinToExistedRoute(waste, car);
        }
    }
    else if(checkUpdateFullness == -1){
        if(waste.fullness >= 70){
            var list = await wasteController.checkForRouting(waste.area_id);
            if(list.length != 0){
                var area = await areaController.getById(waste.area_id);
                var depot = {
                    latitude: area.location[0],
                    longitude: area.location[1]
                };
                var penRecs = await mongoose.model('Penalty_record').find();
                var pop = new Population(list, car, depot, 10, 20, penRecs);
                var bestGene = await pop.generatePopulation();
                var routeTrucks = bestGene[1].getRouteForEachTruck();
                // console.log(waste.name);
                // for(var i = 0; i < routeTrucks.length; i++){
                //     console.log("Truck "+i+": ");
                //     for(var j = 0; j < routeTrucks[i].length; j++){
                //         console.log(routeTrucks[i][j].name);
                //     }
                // }
                for(var i = 0; i < routeTrucks.length; i++){
                    if(routeTrucks[i].length > 2){
                        await carController.updateRoute(car[i]._id, routeTrucks[i]);
                    }
                }
            }
        }
    }
}

wasteStream.on('change', async (next) =>{
    //var waste = await mongoose.model('Waste').findById(next.documentKey._id);
    /*console.log(next.documentKey);
    console.log(waste.name);
    console.log(next.updateDescription.updatedFields);*/
    var key = next.documentKey;
    var dataUpdated = next.updateDescription.updatedFields;

    io.emit('waste change', {
        id: key,
        data: dataUpdated
    });

    if(dataUpdated.fullness != null){
        var waste = await mongoose.model('Waste').findById(next.documentKey._id);
        var car = await carController.getByAreaId(waste.area_id);
        var checkUpdateFullness = await carController.findAndUpdateBinFullness(waste, car);
        if(checkUpdateFullness != 1){
            ableProcess++;
            setTimeout(async () =>{
                ableProcess--;
                if(ableProcess == 0){
                    await fullnessChangeProcessing(waste);
                    while(messQueue.length > 0){
                        await fullnessChangeProcessing(messQueue[0]);
                        messQueue.splice(0,1);
                    }
                    
                }
                else{
                    messQueue.push(waste);
                }
            }, 5000);    
        }
        
        
    
        // var path = car[0].routing;
        // var index = path.findIndex(e => JSON.stringify(e._id) === JSON.stringify(waste._id));
        // var start = car[0].position;
        // var dest = {
        //     latitude: area.location[0],
        //     longitude: area.location[1]
        // };
        // var penRec = await mongoose.model('Penalty_record').find();
        // path.splice(0, 1, start);
        // if(index != -1){
        //     if(waste.fullness == 0){
        //         path.splice(index, 1);
        //     }
        //     else{
        //         path[index].fullness = waste.fullness;
        //     }
        //     await carController.updateRoute(car[0]._id, path);
        // }
        // else{
        //     /*if(waste.fullness >= 70){
        //         path = [start];
        //         if(algorithm.checkForRouting(list, path)){
        //             path = await algorithm.routing(path);
        //             path.push(dest);
        //             await carController.updateRoute(car[0]._id, path);
        //         }
        //     }*/
        //     if(waste.fullness >= 70){
        //         path = [start];
        //         if(algorithm2.checkForRouting(list, path)){
        //             path = await algorithm2.routing(path, penRec);
        //             path.push(dest);
        //             console.log(path);
        //             await carController.updateRoute(car[0]._id, path);
        //         }
        //     }
        // }
        
        //changed for simulating
        if(dataUpdated.fullness >= 90){
            var record = await mongoose.model('Penalty_record').findOne({
                bin_id: key._id.toString()
            });
            if(record == null){
                var recordTime = new Date();
                recordTime.setDate(recordTime.getDate() + iSimulate);
                await mongoose.model('Penalty_record').create({
                    bin_id: key._id.toString(),
                    time: recordTime
                });
            }
        }
        else if(dataUpdated.fullness == 0){
            var record = await mongoose.model('Penalty_record').findOne({
                bin_id: key._id.toString()
            });
            if(record != null){
                await mongoose.model('Penalty_record').deleteOne(record);
                await wasteController.updatePenaltyTime(key._id, record.time, iSimulate);
            }
            else{
                await wasteController.updatePenaltyTime(key._id, null, iSimulate);
            }
        }
    }
    

});

carStream.on('change', async (next) =>{
    if(next.updateDescription.updatedFields.routing != null){
        var route = next.updateDescription.updatedFields.routing;
        var dataChange = {
            id: next.documentKey._id,
            route: route 
        }
        io.emit('car change', {
            data: dataChange
        });
    }
    else if(next.updateDescription.updatedFields['position.latitude'] != null || next.updateDescription.updatedFields['position.longitude'] != null){
        var dataChange = {
            id: next.documentKey._id,
            position: {
                latitude: next.updateDescription.updatedFields['position.latitude'],
                longitude: next.updateDescription.updatedFields['position.longitude']
            }
        }
        io.emit('position change', {
            data: dataChange
        });
    }
});

app.use('/manager', managerRouter);
app.use('/collector', collectorRouter);
app.use('/account', accRouter);
app.use('/user', userRouter);