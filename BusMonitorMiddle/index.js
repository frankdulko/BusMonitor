// initialize express and the server, and a fetch instance too:
const express = require("express");
const server = express();
const fetch = require("node-fetch");

// value for the API request:
let requestUrl = "https://bustime.mta.info/api/siri/stop-monitoring.json";
// and the query string:
let myQuery = {
  key: '019d0445-37d4-4549-8714-47c3e5ea9ba7',
  OperatorRef: 'MTA',
  MonitoringRef: '305103'
};

// set the fetch parameters:
const params = {
  method: "GET",
  headers: {
    accept: "application/json",
  },
};

// a listener for clients making a GET request:
function handleGet(request, response) {
  // iterate over the request query:
  for (let p in request.query) {
    // if there's a matching property in the query,
    if (myQuery.hasOwnProperty(p)) {
      // then replace it with this new value:
      myQuery[p] = request.query[p];
    }
  }
  // convert the query to a string and attach to URL:
  let thisUrl = requestUrl;
  // convert myQuery from JSON to a search param string:
  let queryString = new URLSearchParams(myQuery).toString();
  thisUrl += "?" + queryString;

  // make a request to the remote API using fetch:
  fetch(thisUrl, params)
    // convert fetchResponse to JSON
    .then((fetchResponse) => fetchResponse.json())
    // get the body of the fetchResponse and send to summarize:
    .then((data) => summarize(data))
    // if there's an error, send it to the client:
    .catch((error) => summarize(error));

  // function to do the summary of the result:
  function summarize(result) {
    // if the remote API returns an error:
    if (result.error) {
        response.end(result.error.message);
        return;
      }

      // if there is an error instead of a response:
    if (!result.Siri 
        || !result.Siri.ServiceDelivery 
        || !result.Siri.ServiceDelivery.StopMonitoringDelivery
        || !result.Siri.ServiceDelivery.StopMonitoringDelivery[0]) {
        response.end("ERROR");
        console.log(JSON.stringify(result))
        return;
    }
    
    let data = result.Siri.ServiceDelivery.StopMonitoringDelivery[0].MonitoredStopVisit;
    //console.log(data);
    response.end(JSON.stringify(data));
    // let stopData;

    for (let i = 0; i < data.length(); i++){
        console.log(d[i]);
    }

    // console.log(data);
    //   // if there's a successful result from the remote API:
    //   response.end("Hello");
      //let myData = result.data[0];
    //   let dataToReturn = {};
    //   // get just the time and the water level:
    //   dataToReturn.time = myData.t;
    //   dataToReturn.waterLevel = myData.v;
    //   // send this in response to the original request:
    //   //response.json(dataToReturn);
    //   response.end(JSON.stringify(data))
  }
}

// start the server listening on the glitch default,
// or port 8080:
server.listen(process.env.PORT || 8080);
// define the API endpoints:
server.get("/", handleGet);