//
//  ConversionViewController.swift
//  ch03-1692136-convert
//
//  Created by 이준복 on 2022/03/16.
//

import UIKit

class ConversionViewController: UIViewController {

    @IBOutlet weak var fahrenheitTextField: UITextField!
    @IBOutlet weak var celsiusLabel: UILabel!
    @IBOutlet weak var baseView: UIView!
    
    override func loadView() {
        super.loadView()
        print("loadView")
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        print("viewDidLoad")
        fahrenheitTextField.delegate = self
        
        let tapGesture = UITapGestureRecognizer(target: self, action: #selector(dismissKeyboard))
//        celsiusLabel.addGestureRecognizer(tapGesture)
//        baseView.addGestureRecognizer(tapGesture)
        view.addGestureRecognizer(tapGesture)
    }
    
    @objc func dismissKeyboard(sender: UITapGestureRecognizer) {
        fahrenheitTextField.resignFirstResponder()
    }
    
 
    @IBAction func fahrenheitEditingChanged(_ sender: UITextField) {
        if let text = sender.text {
            if let fahrenheitValue = Double(text) {
                let celsiusValue = 5.0 / 9.0 * (fahrenheitValue - 32.0)
                celsiusLabel.text = String.init(format: "%.2f", celsiusValue)
            } else {
                celsiusLabel.text = "???"
            }
        }
    }
    
}

extension ConversionViewController: UITextFieldDelegate {
    func textField(_ textField: UITextField, shouldChangeCharactersIn range: NSRange, replacementString string: String) -> Bool {
        // 기존 문자열에 . 이 있냐
        let existingTextHasDecimalSeparator = textField.text?.range(of: ".")
        // 새로운 입력한 문자가 . 이냐
        let replacementTextHasDecimalSeparator = string.range(of: ".")
        print(string)
        // 만약 기존 문자열에 . 이 있고 새로 입력한 문자가 . 이라면 입력을 취소한다
        if existingTextHasDecimalSeparator != nil && replacementTextHasDecimalSeparator != nil {
            return false
        } else {
            return true
        }
    }
}

