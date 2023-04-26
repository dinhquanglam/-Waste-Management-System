function createPenTimeChart(arr, days){
    var xVal = [];
    var yVal = [];
    var colors = [];
    for(var i = arr.length-1; i >= arr.length-days && i >= 0; i--){
        xVal.push(arr[i].date);
        yVal.push((arr[i].time/60).toFixed(2));
        if(arr[i].time > 10){
            colors.push("red");
        }
        else if(arr[i].time >= 5 && arr[i].time <= 10){
            colors.push("yellow");
        }
        else{
            colors.push("green");
        }
    }
    yVal.push(0);
    var chart = {
        type: "bar",
        data: {
          labels: xVal,
          datasets: [{
            backgroundColor: colors,
            data: yVal
          }]
        },
        options: {
          legend: {display: false},
          title: {
            display: true,
            text: "Unit: minutes"
          }
        }
    };
    return chart;
}

function createDensityChart(arr, days){
    var xVal = [];
    var yVal = [];
    var colors = [];
    for(var i = arr.length-1; i >= arr.length-days && i >= 0; i--){
        xVal.push(arr[i].date);
        yVal.push((arr[i].weight).toFixed(2));
        if(arr[i].weight > 10){
            colors.push("red");
        }
        else if(arr[i].weight >= 5 && arr[i].weight <= 10){
            colors.push("yellow");
        }
        else{
            colors.push("green");
        }
    }
    yVal.push(0);
    var chart = {
        type: "bar",
        data: {
          labels: xVal,
          datasets: [{
            backgroundColor: colors,
            data: yVal
          }]
        },
        options: {
          legend: {display: false},
          title: {
            display: true,
            text: "Unit: lit"
          }
        }
    };
    return chart;
}

function createPtPieChart(allList, days, time){
    var label = [];
    var value = [];
    var color = [];
    var totalVal = 0;
    allList.forEach(e => {
        label.push(e.name);
        var total = 0;
        for(var i = e.penalty_time.length-1; i >= e.penalty_time.length-days && i >= 0; i--){
            total += e.penalty_time[i].time;
        }
        value.push((total/60).toFixed(2));
        var randomColor = "#" + Math.floor(Math.random()*16777215).toString(16);
        color.push(randomColor);
        totalVal += total;
    });
    return {
        type: 'doughnut',
        data: {
          labels: label,
          datasets: [
            {
                backgroundColor: color,
                data: value,
            },
          ],
        },
        options: {
            events: false,
            animation: {
                duration: 500,
                easing: "easeOutQuart",
                onComplete: function () {
                    var ctx = this.chart.ctx;
                    ctx.font = "bold 1em Montserrat";
                    ctx.textAlign = 'center';
                    ctx.textBaseline = 'bottom';
                    console.log(this.data.datasets);
                    this.data.datasets.forEach(function (dataset) {
                        for (var i = 0; i < dataset.data.length; i++) {
                            var model = dataset._meta[Object.keys(dataset._meta)[0]].data[i]._model,
                                total = dataset._meta[Object.keys(dataset._meta)[0]].total,
                                mid_radius = model.innerRadius + (model.outerRadius - model.innerRadius)/2,
                                start_angle = model.startAngle,
                                end_angle = model.endAngle,
                                mid_angle = start_angle + (end_angle - start_angle)/2;
                            var x = mid_radius * Math.cos(mid_angle);
                            var y = mid_radius * Math.sin(mid_angle);
                            ctx.fillStyle = '#fff';
                            var percent = String(Math.round(dataset.data[i]/total*100)) + "%"; 
                            if(dataset.data[i] != 0 && percent !== "0%" && dataset._meta[time].data[i].hidden != true) {
                                ctx.fillText(dataset.data[i] + " minute", model.x + x, model.y + y);
                                ctx.fillText(percent, model.x + x, model.y + y + 15);
                            }
                        }
                    });               
                }
            }
        },
    }
}

function createDensPieChart(allList, days, time){
    var label = [];
    var value = [];
    var color = [];
    var totalVal = 0;
    allList.forEach(e => {
        label.push(e.name);
        var total = 0;
        for(var i = e.density.length-1; i >= e.density.length-days && i >= 0; i--){
            total += e.density[i].weight;
        }
        value.push((total).toFixed(2));
        var randomColor = "#" + Math.floor(Math.random()*16777215).toString(16);
        color.push(randomColor);
        totalVal += total;
    });
    return {
        type: 'doughnut',
        data: {
          labels: label,
          datasets: [
            {
                backgroundColor: color,
                data: value,
            },
          ],
        },
        options: {
            events: false,
            animation: {
                duration: 500,
                easing: "easeOutQuart",
                onComplete: function () {
                    var ctx = this.chart.ctx;
                    ctx.font = "bold 1em Montserrat";
                    ctx.textAlign = 'center';
                    ctx.textBaseline = 'bottom';
                    console.log(this.data.datasets);
                    this.data.datasets.forEach(function (dataset) {
                        for (var i = 0; i < dataset.data.length; i++) {
                            var model = dataset._meta[Object.keys(dataset._meta)[0]].data[i]._model,
                                total = dataset._meta[Object.keys(dataset._meta)[0]].total,
                                mid_radius = model.innerRadius + (model.outerRadius - model.innerRadius)/2,
                                start_angle = model.startAngle,
                                end_angle = model.endAngle,
                                mid_angle = start_angle + (end_angle - start_angle)/2;
                            var x = mid_radius * Math.cos(mid_angle);
                            var y = mid_radius * Math.sin(mid_angle);
                            ctx.fillStyle = '#fff';
                            var percent = String(Math.round(dataset.data[i]/total*100)) + "%"; 
                            if(dataset.data[i] != 0 && percent !== "0%" && dataset._meta[time].data[i].hidden != true) {
                                ctx.fillText(dataset.data[i] + " lit", model.x + x, model.y + y);
                                ctx.fillText(percent, model.x + x, model.y + y + 15);
                            }
                        }
                    });               
                }
            }
        },
    }
}

function createDistPieChart(allList, time){
    var label = [];
    var value = [];
    var color = [];
    var totalVal = 0;
    allList.forEach(e => {
        label.push(e.name);
        var total = 0;
        e.opDist.forEach(el => {
            total += el.distance;
        });
        value.push(total.toFixed(2));
        var randomColor = "#" + Math.floor(Math.random()*16777215).toString(16);
        color.push(randomColor);
        totalVal += total;
    });
    return {
        type: 'doughnut',
        data: {
          labels: label,
          datasets: [
            {
                backgroundColor: color,
                data: value,
            },
          ],
        },
        options: {
            events: false,
            animation: {
                duration: 500,
                easing: "easeOutQuart",
                onComplete: function () {
                    var ctx = this.chart.ctx;
                    ctx.font = "bold 1em Montserrat";
                    ctx.textAlign = 'center';
                    ctx.textBaseline = 'bottom';
                    console.log(this.data.datasets);
                    this.data.datasets.forEach(function (dataset) {
                        for (var i = 0; i < dataset.data.length; i++) {
                            var model = dataset._meta[Object.keys(dataset._meta)[0]].data[i]._model,
                                total = dataset._meta[Object.keys(dataset._meta)[0]].total,
                                mid_radius = model.innerRadius + (model.outerRadius - model.innerRadius)/2,
                                start_angle = model.startAngle,
                                end_angle = model.endAngle,
                                mid_angle = start_angle + (end_angle - start_angle)/2;
                            var x = mid_radius * Math.cos(mid_angle);
                            var y = mid_radius * Math.sin(mid_angle);
                            ctx.fillStyle = '#fff';
                            var percent = String(Math.round(dataset.data[i]/total*100)) + "%"; 
                            if(dataset.data[i] != 0 && dataset._meta[time].data[i].hidden != true) {
                                ctx.fillText(dataset.data[i] + " km", model.x + x, model.y + y);
                                ctx.fillText(percent, model.x + x, model.y + y + 15);
                            }
                        }
                    });               
                }
            }
        },
    }
}

function createDistChart(arr){
    var xVal = [];
    var yVal = [];
    var colors = [];
    arr.forEach(e => {
        xVal.push(e.date);
        yVal.push(e.distance.toFixed(2));
        if(e.distance > 10){
            colors.push("red");
        }
        else if(e.distance >= 5 && e.distance <= 10){
            colors.push("yellow");
        }
        else{
            colors.push("green");
        }
    });
    yVal.push(0);
    var chart = {
        type: "bar",
        data: {
          labels: xVal,
          datasets: [{
            backgroundColor: colors,
            data: yVal
          }]
        },
        options: {
          legend: {display: false},
          title: {
            display: true,
            text: "Unit: km"
          }
        }
    };
    return chart;
}


$(document).ready(function(){
    var time = 3;
    var day = 7;
    var binIndex = null;
    new Chart("ptChart", createPtPieChart(bins, 7, 0));
    document.getElementById("ptTotal").innerHTML = "Total: "+(penTime7D/60).toFixed(2)+" minutes";
    new Chart("densChart", createDensPieChart(bins, 7, 1));
    document.getElementById("densTotal").innerHTML = "Total: "+(dens7D).toFixed(2)+" lit";
    new Chart("distChart", createDistPieChart(cars, 2));
    document.getElementById("distTotal").innerHTML = "Total: "+(dist).toFixed(2)+" km";
    document.getElementById("overallBin").onclick = function(){
        document.getElementById("overAllContent").classList.add("active");
        document.getElementById("eachBinContent").classList.remove("active");
    }
    document.getElementById("overallCar").onclick = function(){
        document.getElementById("ovCarsContent").classList.add("active");
        document.getElementById("eachCarContent").classList.remove("active");
    }
    document.getElementById("timeBins7d").onclick = function(){
        document.getElementById("timeBins30d").classList.remove("active");
        this.classList.add("active");
        document.getElementById("ptTotal").innerHTML = "Total: "+(penTime7D/60).toFixed(2)+" minutes";
        document.getElementById("densTotal").innerHTML = "Total: "+(dens7D).toFixed(2)+" lit";
        document.getElementById("ptChart").remove();
        document.getElementById("densChart").remove();
        var canvas1 = document.createElement('canvas');
        canvas1.setAttribute("id", "ptChart");
        canvas1.setAttribute("style", "width:80%;");
        var canvas2 = document.createElement('canvas');
        canvas2.setAttribute("id", "densChart");
        canvas2.setAttribute("style", "width:80%;");
        document.getElementById("contentPt").append(canvas1);
        document.getElementById("contentDens").append(canvas2);
        new Chart("ptChart", createPtPieChart(bins, 7, time));
        new Chart("densChart", createDensPieChart(bins, 7, time+1));
        time+=2;
    }
    document.getElementById("timeBins30d").onclick = function(){
        document.getElementById("timeBins7d").classList.remove("active");
        this.classList.add("active");
        document.getElementById("ptTotal").innerHTML = "Total: "+(penTime30D/60).toFixed(2)+" minutes";
        document.getElementById("densTotal").innerHTML = "Total: "+(dens30D).toFixed(2)+" lit";
        document.getElementById("ptChart").remove();
        document.getElementById("densChart").remove();
        var canvas1 = document.createElement('canvas');
        canvas1.setAttribute("id", "ptChart");
        canvas1.setAttribute("style", "width:80%;");
        var canvas2 = document.createElement('canvas');
        canvas2.setAttribute("id", "densChart");
        canvas2.setAttribute("style", "width:80%;");
        document.getElementById("contentPt").append(canvas1);
        document.getElementById("contentDens").append(canvas2);
        new Chart("ptChart", createPtPieChart(bins, 30, time));
        new Chart("densChart", createDensPieChart(bins, 30, time+1));
        time+=2;
    }
    const listBin = document.getElementById("listBins");
    for(var i = 0; i < bins.length; i++){
        const li = document.createElement("li");
        const node = document.createTextNode(bins[i].name);
        if(suggest[i] != 0){
            li.setAttribute("style", "color: #ff0000");
            document.getElementById("modeWarn").style.display = "block";
            if(suggest[i] == 1){
                document.getElementById("warning").innerHTML = "This bin is overloaded !";
                document.getElementById("detail").innerHTML = "Density is upto " + densityPerBin[i] + " lit/day while its capacity is only " + bins[i].capacity + " lit.";
            }
            else if(suggest[i] == -1){
                document.getElementById("warning").innerHTML = "This bin has low effiency !";
                document.getElementById("detail").innerHTML = "Density is only "+ densityPerBin[i] +" lit/day, lower than 50% of its capacity ("+ bins[i].capacity +" lit)";
            }
        }
        li.appendChild(node);
        li.onclick = function(){
            document.getElementById("overAllContent").classList.remove("active");
            document.getElementById("eachBinContent").classList.add("active");
            var index = bins.findIndex(e => e.name === li.innerHTML);
            binIndex = index;
            document.getElementById("binName").innerHTML = li.innerHTML;
            document.getElementById("ptBarChart").remove();
            var canvas1 = document.createElement('canvas');
            canvas1.setAttribute("id", "ptBarChart");
            document.getElementById("contentBinPt").append(canvas1);
            new Chart("ptBarChart", createPenTimeChart(bins[index].penalty_time, day));
            document.getElementById("densBarChart").remove();
            var canvas2 = document.createElement('canvas');
            canvas2.setAttribute("id", "densBarChart");
            document.getElementById("contentBinDens").append(canvas2);
            new Chart("densBarChart", createDensityChart(bins[index].density, day));
            time+=2;   
        }
        listBin.appendChild(li);
    }
    document.getElementById("timeBin7d").onclick = function(){
        day = 7;
        document.getElementById("timeBin30d").classList.remove("active");
        this.classList.add("active");
        document.getElementById("ptBarChart").remove();
        var canvas1 = document.createElement('canvas');
        canvas1.setAttribute("id", "ptBarChart");
        document.getElementById("contentBinPt").append(canvas1);
        new Chart("ptBarChart", createPenTimeChart(bins[binIndex].penalty_time, day));
        document.getElementById("densBarChart").remove();
        var canvas2 = document.createElement('canvas');
        canvas2.setAttribute("id", "densBarChart");
        document.getElementById("contentBinDens").append(canvas2);
        new Chart("densBarChart", createDensityChart(bins[binIndex].density, day));
        time+=2;   
    }
    document.getElementById("timeBin30d").onclick = function(){
        day = 30;
        document.getElementById("timeBin7d").classList.remove("active");
        this.classList.add("active");
        document.getElementById("ptBarChart").remove();
        var canvas1 = document.createElement('canvas');
        canvas1.setAttribute("id", "ptBarChart");
        document.getElementById("contentBinPt").append(canvas1);
        new Chart("ptBarChart", createPenTimeChart(bins[binIndex].penalty_time, day));
        document.getElementById("densBarChart").remove();
        var canvas2 = document.createElement('canvas');
        canvas2.setAttribute("id", "densBarChart");
        document.getElementById("contentBinDens").append(canvas2);
        new Chart("densBarChart", createDensityChart(bins[binIndex].density, day));
        time+=2;   
    }
    const listCar = document.getElementById("listCars");
    for(var i = 0; i < cars.length; i++){
        const li = document.createElement("li");
        const node = document.createTextNode(cars[i].name);
        li.appendChild(node);
        li.onclick = function(){
            document.getElementById("ovCarsContent").classList.remove("active");
            document.getElementById("eachCarContent").classList.add("active");
            document.getElementById("carName").innerHTML = li.innerHTML;
            var index = cars.findIndex(e => e.name === li.innerHTML);
            document.getElementById("distBarChart").remove();
            var canvas = document.createElement('canvas');
            canvas.setAttribute("id", "distBarChart");
            document.getElementById("contentCarDist").append(canvas);
            new Chart("distBarChart", createDistChart(cars[index].opDist));
            time++;
        };
        listCar.appendChild(li);
    }
});