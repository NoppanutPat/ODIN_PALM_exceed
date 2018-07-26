let rain = 0
let light = 0
let temp = 0
let humid = 0
let start = 0
let finish = 0
let test = 0
let drop = "on"
let x = false

let getWeb = (sensor) => {
    return $.ajax({
        type: "GET",
        url: `http://ecourse.cpe.ku.ac.th/exceed/api/odinpalm-${sensor}/view`,
        dataType: "text",
    });
}

let getAll = () => {
    test = getWeb("rain_status").then((response) => {
        rain = parseInt(response)
        if (rain === 0) {
            $('#result-rain-image').html('<img width="76" src="./source/Rainy.svg" alt="">')
            $('#result-rain').html(`<div class="miniText"><br><h3>Rainy</h3></div>`)
        }
        else {
            $('#result-rain-image').html('<img width="76" src="./source/Cloudy.svg" alt="">')
            $('#result-rain').html(`<div class="miniText"><br><h3>Sunny</h3></div>`)
        }
    })

    test = getWeb("light_status").then((response) => {
        light = response
        if (light <= 200) {
            $('#result-light-image').html('<img width="76" src="./source/DayNight.svg" alt="">')
            $('#result-light').html(`<div class="miniText">
            <br>
            <h3>Goodnight</h3>
        </div>`)
        }
        else {
            $('#result-light-image').html('<img width="76" src="./source/Daylight.svg" alt="">')
            $('#result-light').html(`<div class="miniText"><br><h3>Morning</h3>
        </div>`)
        }
    })

    test = getWeb("temp").then((response) => {
        temp = response
        $('#result-temp').html(`<div class="miniText"><br><h3>${temp}°C</h3></div>`)
    })
    test = getWeb("humid").then((response) => {
        humid = parseFloat(response)
        $('#result-humid').html(`<div class="miniText"><br><h3>${humid}%</h3></div>`)
    })
    test = getWeb("start").then((response) => {
        start = response
        $('#result-start').html(`<div style="font-size: 100px;color: black;font-weight: 600" class="centered">${start}</div>`)
    })
    test = getWeb("finish").then((response) => {
        finish = response
        $('#result-finish').html(`<div style="font-size: 100px;color: black;font-weight: 600" class="centered">${finish}</div>`)
    })
}

let getSetup = () => {
    setInterval(getAll, 2000)
}

let postWeb = (sensor, data) => {
    $.ajax({
        type: "POST",
        url: `http://ecourse.cpe.ku.ac.th/exceed/api/odinpalm-${sensor}/set`,
        data: {
            value: data
        },
        dataType: "json",
        success: function (response) {

        }
    });
}

let postSetup = () => {
    $('#auto-drop-button').on('click', () => {
        if (x) {
            postWeb('drop_status', 1)
        }
    })


    $('#on-round-button').on('mousedown', () => {
        postWeb('round_status', 1)
    })
    $('#on-round-button').on('mouseup', () => {
        postWeb('round_status', 0)
    })
}

let swapSwitch = () => {
    $('#swap').on('click', () => {
        x = !x;
        if (x === true) {
            $('#auto-drop-button').removeClass(`disabled`)
            $('#manual-auto').html(`<h3 style="text-align: right">Manual</h3>`)
            postWeb('drop_auto_status', 1)
        }
        else {
            $('#auto-drop-button').addClass(`disabled`)
            $('#manual-auto').html(`<h3 style="text-align: right">Auto</h3>`)
            postWeb('drop_auto_status', 0)
        }
    })
}

function myFunction() {
    postWeb('drop_status', 0)
}

window.onload = function () {
    var dps = []; // dataPoints
    var chart = new CanvasJS.Chart("chartTemperature", {
        title: {
            text: "Temperature"
        },
        axisY: {
            includeZero: false
        },
        data: [{
            type: "line",
            dataPoints: dps
        }]
    });

    var xVal = 0;
    var yVal = 0;
    var updateInterval = 2000;
    var dataLength = 10; // number of dataPoints visible at any point

    var updateChart = function (count) {

        count = count || 1;

        for (var j = 0; j < count; j++) {
            yVal = parseInt(temp);
            dps.push({
                x: xVal,
                y: yVal
            });
            xVal++;
        }

        if (dps.length > dataLength) {
            dps.shift();
        }

        chart.render();
    };

    updateChart(dataLength);
    setInterval(function () { updateChart() }, updateInterval);

    var dpsTwo = []; // dataPoints
    var chartTwo = new CanvasJS.Chart("chartMoisture", {
        title: {
            text: "Moisture"
        },
        axisY: {
            includeZero: false
        },
        data: [{
            type: "line",
            dataPoints: dpsTwo
        }]
    });

    var xValTwo = 0;
    var yValTwo = 0;
    var updateIntervalTwo = 2000;
    var dataLengthTwo = 20; // number of dataPoints visible at any point

    var updateChartTwo = function (countThree) {

        countThree = countThree || 1;

        for (var j = 0; j < countThree; j++) {
            yValTwo = humid;
            dpsTwo.push({
                x: xValTwo,
                y: yValTwo
            });
            xValTwo++;
        }

        if (dpsTwo.length > dataLength) {
            dpsTwo.shift();
        }

        chartTwo.render();
    };

    updateChartTwo(dataLengthTwo);
    setInterval(function () { updateChartTwo() }, updateIntervalTwo);
}

let init = () => {
    getSetup()
    postSetup()
    swapSwitch()
}

$(init)