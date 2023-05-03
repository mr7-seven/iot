document.addEventListener("DOMContentLoaded", function () {
    var chartT = new Highcharts.Chart({
      chart: { renderTo: "chart-temperature", type: "spline" },
      title: { text: "Temperature" },
      series: [
        {
          showInLegend: false,
          data: [],
        },
      ],
      plotOptions: {
        line: { animation: false, dataLabels: { enabled: true } },
        series: { color: "#059e8a" },
      },
      tooltip: {
    formatter: function () {
        return 'Suhu : <b>'+ this.y + '</b>';
     }
    },
      xAxis: {
        type: "datetime",
        dateTimeLabelFormats: { second: "%H:%M:%S" },
      },
      yAxis: {
        title: { text: "Temperature (Celsius)" },
      },
      credits: { enabled: false },
    });

    function updateDataChart() {
      $.getJSON("random_data_chart.php", function (data) {
        // Mendapatkan waktu saat ini dalam format UTC
        var date = new Date();
        var offset = date.getTimezoneOffset();
        var timezoneOffset = offset - 480;
        // Menyesuaikan waktu dengan perbedaan waktu antara UTC dan WITA (UTC+8)
        var x = date.getTime() + timezoneOffset * 60 * 1000;
        // y = parseFloat(data);
        console.log('waktu : '+ x);
        y = data;
        console.log(y);
        if (chartT.series[0].data.length > 40) {
          chartT.series[0].addPoint([x, y], true, true, true);
        } else {
          chartT.series[0].addPoint([x, y], true, false, true);
        }
      });
    }
  });
