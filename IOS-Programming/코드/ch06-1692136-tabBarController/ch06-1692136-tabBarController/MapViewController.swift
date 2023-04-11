//
//  MapViewController.swift
//  ch06-1692136-tabBarController
//
//  Created by 이준복 on 2022/04/08.
//

import UIKit
import MapKit

class MapViewController: UIViewController {

    @IBOutlet weak var mapView: MKMapView!

    
    override func viewDidLoad() {
        super.viewDidLoad()
    }
    
    override func viewWillAppear(_ animated: Bool) {
        let parent = self.parent as! UITabBarController
        let cityViewController = parent.viewControllers![0] as! CityViewController
        let (city, longitute, latitute) = cityViewController.getCurrentLonLat()
        
        updateMap(title: city, longitute: longitute, latitute: latitute)
        
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


