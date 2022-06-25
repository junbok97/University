//
//  ViewController.swift
//  ch05-programmingView
//
//  Created by 이준복 on 2022/03/29.
//

import UIKit

class ConversionViewController: UIViewController {
    
    var fahrenheitTextField: UITextField!
    var celsiusLabel: UILabel!
    
    var isLabel,fdegreeLabel, cdegreeLabel: UILabel!
    var segmentedControl: UISegmentedControl!
    
    var isLabelYConstraintWithoutKeyboard: NSLayoutConstraint?
    var isLabelYConstraintWithKeyboard: NSLayoutConstraint?
    var segmentedControlBottomConstraint: NSLayoutConstraint?

    override func viewDidLoad() {
        super.viewDidLoad()
        
        isLabel = createLabel("is", fontSize: 36)
        isLabel.centerXAnchor.constraint(equalTo: view.safeAreaLayoutGuide.centerXAnchor).isActive = true
        isLabel.centerYAnchor.constraint(equalTo: view.safeAreaLayoutGuide.centerYAnchor).isActive = true
        
        fahrenheitTextField = createTextField(placeHolder: "VALUE")
        fdegreeLabel = createLabel("degrees Fahrenheit", fontSize: 36)
        celsiusLabel = createLabel("???", fontSize: 70)
        cdegreeLabel = createLabel("degrees Celsius", fontSize: 36)
        
        connectVertically(views: fahrenheitTextField,fdegreeLabel,isLabel,celsiusLabel,cdegreeLabel,spacing: 10)
        connectHorizontally(views: fahrenheitTextField,fdegreeLabel,isLabel,celsiusLabel,cdegreeLabel)
        
        fahrenheitTextField.addTarget(self, action: #selector(fahrenheitEditingChaged), for: .editingChanged)
        let tap = UITapGestureRecognizer(target: self, action: #selector(dismissKeyboard))
        view.addGestureRecognizer(tap)
        
        fahrenheitTextField.delegate = self
        
        addSegmentedControl()
        
        isLabelYConstraintWithoutKeyboard = isLabel.centerYAnchor.constraint(equalTo: view.safeAreaLayoutGuide.centerYAnchor)
        isLabelYConstraintWithoutKeyboard?.isActive = true
        isLabelYConstraintWithKeyboard = isLabel.centerYAnchor.constraint(equalTo: view.safeAreaLayoutGuide.centerYAnchor, constant: -200)
        isLabelYConstraintWithKeyboard?.isActive = false
        
        
        
        // NotificationCenter에 옵저버 등록을 한다.
        // 키보드가 나타나면 keyboardWillShow 함수를 호출한다
        NotificationCenter.default.addObserver(self,selector: #selector(keyboardWillShow),
        name: UIResponder.keyboardWillShowNotification, object: nil )
        // 키보드가 사라지면 keyboardWillHide 함수를 호출한다
        NotificationCenter.default.addObserver(self,selector: #selector(keyboardWillHide),
        name: UIResponder.keyboardWillHideNotification,object: nil )
        
        let toolBarKeyboard = UIToolbar()
        toolBarKeyboard.sizeToFit()
        let btnDoneBar = UIBarButtonItem(title: "Done", style: .done, target: self, action: #selector(self.dismissKeyboard))
        let space = UIBarButtonItem(barButtonSystemItem: .flexibleSpace, target: nil, action: nil)
        toolBarKeyboard.items = [space, btnDoneBar]
        toolBarKeyboard.tintColor = .green
        fahrenheitTextField.inputAccessoryView = toolBarKeyboard
    }
    
}



extension ConversionViewController{
    @objc private func keyboardWillShow(_ notification: Notification) {
        // 이것은 keyboardWillShow가 두번 연속호축되는 경우 그냥 리턴
        if isLabelYConstraintWithKeyboard?.isActive == true{ return
        }
        isLabelYConstraintWithoutKeyboard?.isActive = false
        isLabelYConstraintWithKeyboard?.isActive = true
        segmentedControlBottomConstraint?.constant -= 350
    }
    
    @objc private func keyboardWillHide(_ notification: Notification) {
        isLabelYConstraintWithoutKeyboard?.isActive = true
        isLabelYConstraintWithKeyboard?.isActive = false
        segmentedControlBottomConstraint?.constant += 350
    }
    
}


// addSegmentedControll
extension ConversionViewController {
    func addSegmentedControl() {
        segmentedControl = UISegmentedControl(items: ["Fahrenheit", "Celsius"])
        let font = UIFont.systemFont(ofSize: 20)
        segmentedControl!.setTitleTextAttributes([NSAttributedString.Key.font: font], for: .normal)
        segmentedControl.selectedSegmentIndex = 0
        view.addSubview(segmentedControl)
        segmentedControl.translatesAutoresizingMaskIntoConstraints = false
        
        segmentedControl.leadingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.leadingAnchor, constant: 10).isActive = true
        segmentedControl.trailingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.trailingAnchor, constant: -10).isActive = true
//        segmentedControl.bottomAnchor.constraint(equalTo: view.safeAreaLayoutGuide.bottomAnchor, constant: -10).isActive = true
        
        segmentedControlBottomConstraint = segmentedControl.bottomAnchor.constraint(equalTo: view.safeAreaLayoutGuide.bottomAnchor, constant: -10)
        segmentedControlBottomConstraint?.isActive = true
        
        segmentedControl.addTarget(self, action: #selector(changeDegrees), for: .valueChanged)
    }
}

// @objc func
extension ConversionViewController {
    @objc func changeDegrees(sender: UISegmentedControl){
        if sender.selectedSegmentIndex == 0 {
            fdegreeLabel.text = "degrees Fahrenheit"
            cdegreeLabel.text = "degrees Celsius"
        } else {
            fdegreeLabel.text = "degrees Celsius"
            cdegreeLabel.text = "degrees Fahrenheit"
        }
        fahrenheitTextField.text = ""
        celsiusLabel.text = "???"
    }
    
    @objc func dismissKeyboard(sender: Any) {
        fahrenheitTextField.resignFirstResponder()
    }
    
    @objc func fahrenheitEditingChaged(sender: UITextField) {
        if let text = sender.text {
            if let fahrenheitValue = Double(text){
                if segmentedControl.selectedSegmentIndex == 0 {
                    let celsiusValue = 5.0/9.0*(fahrenheitValue - 32.0)
                    celsiusLabel.text = String.init(format: "%.2f", celsiusValue)
                } else {
                    let celsiusValue = 9.0/5.0*fahrenheitValue + 32.0
                    celsiusLabel.text = String.init(format: "%.2f", celsiusValue)
                }
            } else {
                celsiusLabel.text = "???"
            }
        }
    }

}

// Connect
extension ConversionViewController {
    func connectVertically(views: UIView..., spacing: CGFloat){
        for i in 0..<views.count-1 {
            views[i].bottomAnchor.constraint(equalTo: views[i+1].topAnchor, constant: spacing).isActive = true
        }
    }
    
    func connectHorizontally(views: UIView...) {
        for view in views{
            view.centerXAnchor.constraint(equalTo: self.view.safeAreaLayoutGuide.centerXAnchor).isActive = true
        }
    }
}

// Create UITextField, UILabel
extension ConversionViewController {
    func createTextField(placeHolder: String) -> UITextField{
        let textField = UITextField(frame:  CGRect())
        textField.textAlignment = .center
        textField.placeholder = placeHolder
        textField.font = UIFont(name: textField.font!.fontName, size: 70)
        textField.keyboardType = .decimalPad
        view.addSubview(textField)
        textField.translatesAutoresizingMaskIntoConstraints = false
        return textField
    }
    
    func createLabel(_ text: String, fontSize: CGFloat) -> UILabel {
        let label = UILabel(frame: CGRect())
        label.text = text
        label.textColor = UIColor(red: CGFloat(0xe1)/CGFloat(256), green: CGFloat(0x58)/CGFloat(256), blue: CGFloat(0x29)/CGFloat(256), alpha: CGFloat(1))
        label.textAlignment = .center
        label.font = UIFont(name: label.font!.fontName, size: fontSize)
        view.addSubview(label)
        label.translatesAutoresizingMaskIntoConstraints = false
        return label
    }
}



// UITextFieldDelegate
extension ConversionViewController: UITextFieldDelegate {
    func textField(_ textField: UITextField, shouldChangeCharactersIn range: NSRange, replacementString string: String) -> Bool {
        let existingTextHasDecimalSeparator = textField.text?.range(of: ".")
        let replacementTextHasDecimalSeparator = string.range(of: ".")
        if existingTextHasDecimalSeparator != nil && replacementTextHasDecimalSeparator != nil {
            return false
        } else {
            return true
        }
    }
}



//extension ConversionViewController {
//    override func viewDidLoad() {
//        super.viewDidLoad()
//
//        let helloLabel = UILabel(frame: CGRect(x: 100, y: 100, width: 200, height: 30))
//        helloLabel.text = "Hello, Autolayout"
//        helloLabel.backgroundColor = UIColor.green
//        helloLabel.textAlignment = .center
//        view.addSubview(helloLabel)
//
//        helloLabel.translatesAutoresizingMaskIntoConstraints = false
//        helloLabel.topAnchor.constraint(equalTo: view.safeAreaLayoutGuide.topAnchor, constant: 100).isActive = true
//        helloLabel.centerXAnchor.constraint(equalTo: view.safeAreaLayoutGuide.centerXAnchor).isActive = true
//    }
//}
