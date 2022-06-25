//
//  PlanDetailViewController.swift
//  ch10-1692136-stackView
//
//  Created by 이준복 on 2022/05/08.
//

import UIKit

class PlanDetailViewController: UIViewController {

    @IBOutlet weak var dateDatePicker: UIDatePicker!
    @IBOutlet weak var ownerLabel: UILabel!
    @IBOutlet weak var typePickerView: UIPickerView!
    @IBOutlet weak var contentTextView: UITextView!
    
    var plan: Plan?
    var saveChangeDelegate: ((Plan) -> Void)?
    
    override func viewDidLoad() {
        super.viewDidLoad()

        plan = plan ?? Plan(date: Date(), withData: true)
        
        typePickerView.dataSource = self
        typePickerView.delegate = self
        
        dateDatePicker.date = plan!.date
        ownerLabel.text = plan?.owner
        typePickerView.selectRow(plan!.kind.rawValue, inComponent: 0, animated: false)
        contentTextView.text = plan?.content
        
        let tap = UITapGestureRecognizer(target: self, action: #selector(dismissKeyboard))
        view.addGestureRecognizer(tap)
    }
    
    @objc func dismissKeyboard(sender: UITapGestureRecognizer) {
        contentTextView.resignFirstResponder()
    }
    
    @IBAction func gotoBack(_ sender: UIButton) {
        plan!.date = dateDatePicker.date
        plan?.owner = ownerLabel.text
        plan?.kind = Plan.Kind(rawValue: typePickerView.selectedRow(inComponent: 0))!
        plan?.content = contentTextView.text
        
        saveChangeDelegate?(plan!)
        
        dismiss(animated: true, completion: nil)
    }
    
}

extension PlanDetailViewController: UIPickerViewDelegate, UIPickerViewDataSource {
    
    func numberOfComponents(in pickerView: UIPickerView) -> Int {
        return 1
    }
    
    func pickerView(_ pickerView: UIPickerView, numberOfRowsInComponent component: Int) -> Int {
        return Plan.Kind.count
    }
    
    func pickerView(_ pickerView: UIPickerView, titleForRow row: Int, forComponent component: Int) -> String? {
        let type = Plan.Kind(rawValue: row)
        return type!.toString()
    }
}
