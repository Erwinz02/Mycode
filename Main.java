package sample;

import javafx.collections.FXCollections;
import javafx.scene.chart.LineChart;
import javafx.scene.chart.NumberAxis;
import javafx.scene.chart.XYChart;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import sample.SerialPortService;
import javafx.application.Application;
import javafx.geometry.Insets;
import javafx.scene.Scene;
import javafx.scene.control.Slider;
import javafx.scene.layout.BorderPane;
import javafx.stage.Stage;
import java.io.IOException;


public class Main extends Application {

    public static void main(String[] args) {
        launch(args);

    }
    private final static int MAX_VALUE = 1 << 10;
    @Override
    public void start(Stage primaryStage) {
        var sp = SerialPortService.getSerialPort("COM3");
        var outputStream = sp.getOutputStream();

        var pane = new BorderPane();

        var button = new Button();
        var label = new Label();
        var slider = new Slider();
        slider.setMin(0.0);
        slider.setMax(100.0);

        var controller = new DataController(); // create the controller

        sp.addDataListener(controller);

        primaryStage.setTitle("ADC Moisture Value from Moisture Sensor vs Time");

        var now = System.currentTimeMillis();


        var xAxis = new NumberAxis("time (ms since Jan 1, 1970)", now, now + 50000, 10000); // creates the x-axis (which automatically updates)
        var yAxis = new NumberAxis("ADC Moisture Value", 0, MAX_VALUE, 10); // creates the y-axis

        var series = new XYChart.Series<>(controller.getDataPoints()); // creates the series (all the data)
        var lineChart = new LineChart<>(xAxis, yAxis, FXCollections.singletonObservableList(series)); // creates the chart
        lineChart.setTitle("Moisture Sensor");

        Scene scene = new Scene(lineChart, 1000, 600); // creates the JavaFX window

        primaryStage.setScene(scene);
        primaryStage.show();


        button.setOnMouseReleased(value -> {

            try {
                outputStream.write(0);
            } catch (IOException e) {
                e.printStackTrace();
            }
        });

        button.setOnMousePressed(value -> {
            try {
                outputStream.write(255);
            } catch (IOException e) {
                e.printStackTrace();
            }
        });

        slider.valueProperty().addListener((observableValue, oldValue, newValue) -> {
            label.setText(String.valueOf(newValue.intValue()));
        });


        slider.valueProperty().addListener((observableValue, oldValue, newValue) -> {
            try {
                outputStream.write(newValue.byteValue());
            } catch (IOException e) {
                e.printStackTrace();
            }
        });

        pane.setBottom(label);
        pane.setTop(button);

        pane.setBottom(slider);
        pane.setCenter(lineChart);
        pane.setPadding(new Insets(0, 20, 0, 20));

        var scene2 = new Scene(pane, 1000, 800);

        primaryStage.setScene(scene2);
        primaryStage.show();


    }
}