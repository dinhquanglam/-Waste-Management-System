var wasteController = require('./controllers/wasteController');

var simulate = {
    main: async function(){
        var list = await wasteController.getAll();
        var i = 0;
        await simulate.timerPer1m(i, list);
    },

    timerPer20s: function(i, list){
        var start = new Date().getTime();
        var timer = setInterval(() => {
            const current = new Date().getTime();
            if(current - start >= 60000){
                clearInterval(timer);
                i++;
                console.log(i + " hour(s) ended");
                return;
            }
            var x = 0;
            async function loop () {
                if(list[x].name !== 'waste8' && list[x].name !== 'waste9' && list[x].name !== 'waste10'){
                    await wasteController.updateFullness(list[x].id, simulate.wasteDensity(i));
                }
                x++;

                if (x<list.length) {
                    setTimeout(loop, 1000);
                }
            }
            loop();
            // list.forEach((e) => {
            //     setTimeout(async () =>{
            //         if(e.name !== 'waste8' && e.name !== 'waste9' && e.name !== 'waste10'){
            //             await wasteController.updateFullness(e.id, simulate.wasteDensity(i));
            //         }
            //     }, 2000);
            // });
        }, 20000);
        return (i+1)%24;
    },
    
    timerPer1m: async function(i, list){
        i = await simulate.timerPer20s(i, list);
        setInterval(async () => {
            i = await simulate.timerPer20s(i, list);
        }, 60000);
        return i;
    },
    
    randomRange: function(min, max){
        return Math.random() * (max - min) + min;
    },
    
    wasteDensity: function(i){
        var w = 0;
        if(i >= 0 && i <= 5){
            w = simulate.randomRange(5/3, 10/3);
        }
        else if(i >= 6 && i <= 9){
            w = simulate.randomRange(30/3, 50/3);
        }
        else if(i >= 10 && i <= 16){
            w = simulate.randomRange(50/3, 70/3);
        }
        else if(i >= 17 && i <= 21){
            w = simulate.randomRange(80/3, 100/3);
        }
        else if(i >= 22 && i <= 23){
            w = simulate.randomRange(10/3, 20/3);
        }
        return w;
    }
};

module.exports = simulate;

