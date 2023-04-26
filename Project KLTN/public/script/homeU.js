function creatMarker(loc){
    var markerIcon = null;
    if(loc.fullness != null){
        if(loc.fullness <= 30){
        markerIcon = greenIcon;
        }
        else if(loc.fullness > 30 && loc.fullness < 70){
            markerIcon = yellowIcon;
        }
        else{
            markerIcon = redIcon;
        }
    }
    var marker = null;
    if(markerIcon == null){
        marker = L.marker([loc.latitude, loc.longitude]);    
    }
    else{
        marker = L.marker([loc.latitude, loc.longitude], {icon: markerIcon}).on("click", function(e){
            const infor = document.getElementById("infor");
            infor.style.display = "block";
            document.getElementById("name").innerHTML = loc.name;
            document.getElementById("cap").innerHTML = loc.fullness.toFixed(2);
            document.getElementById("nameInput").value = loc.name;
            document.getElementById("capInput").value = loc.fullness;
            document.getElementById("idInput").value = JSON.stringify(loc._id);
        });
    }
    
    return marker;
}

function distanceBetween2Node(start, dest){
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

function findNearest(loc, yLat, yLng){
    var yourLoc = {
        latitude: yLat,
        longitude: yLng
    };
    var destLoc = null;
    var minDist = Infinity;
    loc.forEach(function(e){
        var temp = distanceBetween2Node(yourLoc, e);
        if(temp < minDist && e.fullness < 70){
            destLoc = e;
            minDist = temp;
        }
    });
    return destLoc;
}

var LeafIcon = L.Icon.extend({
    options: {
        iconSize:     [38, 40],
        iconAnchor:   [19, 40],
        popupAnchor:  [0, -40]
    }
});
var greenIcon = new LeafIcon({iconUrl: './Icon/green-bin-icon.png'});
var yellowIcon = new LeafIcon({iconUrl: './Icon/yellow-bin-icon.png'});
var redIcon = new LeafIcon({iconUrl: './Icon/red-bin-icon.png'});

$(document).ready(async function(){
    var socket = io();
    var cMarker, cLat, cLng;
    var map = L.map("map");
    if (!navigator.geolocation) {
        alert("Your browser doesn't support geolocation feature!")
    } else {
        navigator.geolocation.getCurrentPosition(async function getPosition(position) {
            cLat = position.coords.latitude;
            cLng = position.coords.longitude;
            if (cMarker) {
                map.removeLayer(cMarker);
            }
            cMarker = L.marker([cLat, cLng]).addTo(map);
            map.setView([cLat, cLng], 17);
        });
        setInterval(async () => {
            navigator.geolocation.getCurrentPosition(async function getPosition(position) {
                cLat = position.coords.latitude;
                cLng = position.coords.longitude;
                if (cMarker) {
                    map.removeLayer(cMarker);
                }
                cMarker = L.marker([cLat, cLng]).addTo(map);
            });
        }, 5000);
    }
    map.on("click", () =>{
        if(document.getElementById("infor").style.display === "block"){
            document.getElementById("infor").style.display = "none"
        }
    });
    L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
        maxZoom: 19,
        attribution: '© OpenStreetMap'
    }).addTo(map);

    loc.forEach(function(lc){
        creatMarker(lc).addTo(map);
    });
    var route = L.Routing.control({
        show: false,
        draggableWaypoints: false,
        lineOptions: {
            styles: [{color: 'blue', opacity: 1, weight: 2}]
        },
        createMarker: function(i, start, n){
            return null;
        }
    }).addTo(map);

    document.getElementById("findNearestButton").onclick = function(){
        var nearestLoc = findNearest(loc, cLat, cLng);
        if(nearestLoc != null){
            route.setWaypoints([{lat: cLat, lng: cLng}, {lat: nearestLoc.latitude, lng: nearestLoc.longitude}]);
            map.setView([nearestLoc.latitude, nearestLoc.longitude], 17);
        }
    }

    socket.on('waste change', async function(msg){
        var locI = loc.findIndex(e => JSON.stringify(e._id) === JSON.stringify(msg.id._id));
        console.log(loc);
        if(msg.data.fullness != null){
            let newCap = Number(msg.data.fullness);
            loc[locI].fullness = newCap;
            map.eachLayer(function(layer){
                if(layer._latlng != null && layer._latlng.lat == loc[locI].latitude && layer._latlng.lng == loc[locI].longitude){
                    console.log("141");
                    map.removeLayer(layer);
                    return;
                }
            });
            creatMarker(loc[locI]).addTo(map);
        }

        if(infor.style.display === "block" && document.getElementById("idInput").value === JSON.stringify(msg.id._id)){
            if(msg.data.fullness != null){
                document.getElementById("cap").innerHTML = Number(msg.data.fullness).toFixed(2);
                document.getElementById("capInput").value = Number(msg.data.fullness);
            }
        }
    });

});