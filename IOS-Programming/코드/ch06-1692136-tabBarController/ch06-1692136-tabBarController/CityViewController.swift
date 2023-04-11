//
//  CityViewController.swift
//  ch06-1692136-tabBarController
//
//  Created by 이준복 on 2022/04/08.
//

import UIKit

class CityViewController: UIViewController {

    @IBOutlet weak var cityPickerView: UIPickerView!
    
    var cities: [String: [String:Double]] = [
        "Seoul" : ["lon":126.9778,"lat":37.5683], "Athens" : ["lon":23.7162,"lat":37.9795],
        "Bangkok" : ["lon":100.5167,"lat":13.75], "Berlin" : ["lon":13.4105,"lat":52.5244],
        "Jerusalem" : ["lon":35.2163,"lat":31.769], "Lisbon" : ["lon":-9.1333,"lat":38.7167],
        "London" : ["lon":-0.1257,"lat":51.5085], "New York" : ["lon":-74.006,"lat":40.7143],
        "Paris" : ["lon":2.3488,"lat":48.8534], "Sydney" : ["lon":151.2073,"lat":-33.8679]
    ]

    
    override func viewDidLoad() {
        super.viewDidLoad()
        cityPickerView.dataSource = self
        cityPickerView.delegate = self
        print("viewdidLoad")

    }
    

   
}

extension CityViewController: UIPickerViewDataSource {
    // 피커가 몇개인지
    func numberOfComponents(in pickerView: UIPickerView) -> Int {
        return 1
    }
    
    // 피커에 들어가는 데이터가 몇개인지
    func pickerView(_ pickerView: UIPickerView, numberOfRowsInComponent component: Int) -> Int {
        let cityNames = Array(cities.keys)
        return cityNames.count
    }
}

extension CityViewController: UIPickerViewDelegate {
    // 타이틀리턴
    func pickerView(_ pickerView: UIPickerView, titleForRow row: Int, forComponent component: Int) -> String? {
        var cityNames = Array(cities.keys)
        cityNames.sort()
        
        return cityNames[row]
    }
}


extension CityViewController {
    func getCurrentLonLat() -> (String, Double?, Double?){
        var cityNames = Array(self.cities.keys)
        cityNames.sort()
        let selectedCity = cityNames[self.cityPickerView.selectedRow(inComponent: 0)]
        let city = self.cities[selectedCity]
        return (selectedCity, city!["lon"], city!["lat"])
    }
}
