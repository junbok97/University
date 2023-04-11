//
//  MapViewController.swift
//  ch06-1692136-tabBarController
//
//  Created by 이준복 on 2022/04/08.
//

import UIKit
import MapKit
import Progress

class MapViewController: UIViewController {

    @IBOutlet weak var mapView: MKMapView!
    let baseURLString = "https://api.openweathermap.org/data/2.5/weather"
    let apiKey = "d462688dd5c10b6f961c3bb27b5364e6"
    
    
    override func viewDidLoad() {
        super.viewDidLoad()
    }
    
    override func viewWillAppear(_ animated: Bool) {
        let parent = self.parent as! UITabBarController
        let cityViewController = parent.viewControllers![0] as! CityViewController
        let (city, longitute, latitute) = cityViewController.getCurrentLonLat()
        
        updateMap(title: city, longitute: longitute, latitute: latitute)
        
    }
    
    func getWeatherData(cityName city: String){
        Prog.start(in: self, .activityIndicator)
        var urlStr = baseURLString+"?"+"q="+city+"&"+"appid="+apiKey
        urlStr = urlStr.addingPercentEncoding(withAllowedCharacters: .urlQueryAllowed)!
        
        let session = URLSession(configuration: .default)
        let url = URL(string: urlStr)
        let request = URLRequest(url: url!)
        
        let dataTask = session.dataTask(with: request) {
            (data, response, error) in
            guard let jsonData = data else{print(error!); return}
            if let jsonStr = String(data: jsonData, encoding: .utf8) {
                print(jsonStr)
            }
            
            let (temperature, longitute, latitute) = self.extractWeatherData(jsonData: jsonData)
            
            var title = city
            if let temperature = temperature{
                title += String.init(format: ": %2.f℃", temperature)
            }
            DispatchQueue.main.async {
                self.updateMap(title: title, longitute: longitute, latitute: latitute)
                Prog.dismiss(in: self)
            }
        }
        dataTask.resume()
    }
    
    func extractWeatherData(jsonData: Data) -> (Double?, Double?, Double?) {
        let json = try! JSONSerialization.jsonObject(with: jsonData, options: []) as! [String: Any]
        
        if let code = json["cod"] {
            if code is String, code as! String == "404" {
                return (nil,nil,nil)
            }
        }
        
        let latitude = (json["coord"] as! [String: Double])["lat"]
        let longitude = (json["coord"] as! [String: Double])["lon"]
        
        guard var temperature = (json["main"] as! [String: Double])["temp"] else{
            return (nil, longitude, latitude)
        }
        temperature = temperature - 273.0
        return (temperature, longitude, latitude)        
    }
    
    func updateMap(title: String, longitute: Double?, latitute: Double?){

        let span = MKCoordinateSpan(latitudeDelta: 1, longitudeDelta: 1)
        var center = mapView.centerCoordinate
        if let longitute = longitute, let latitute = latitute{
            center = CLLocationCoordinate2D(latitude: latitute, longitude: longitute)
        }
        let region = MKCoordinateRegion(center: center, span: span)
        mapView.setRegion(region, animated: true)
        
        let annotation = MKPointAnnotation()
        annotation.coordinate = center
        annotation.title = title
        mapView.addAnnotation(annotation)
    }

    @IBAction func sgcValueChanged(_ sender: UISegmentedControl) {
        switch sender.selectedSegmentIndex {
        case 0:
            self.mapView.mapType = .standard
        case 1:
            self.mapView.mapType = .hybrid
        case 2:
            self.mapView.mapType = .satellite
        default:
            break
        }
    }
    
}


