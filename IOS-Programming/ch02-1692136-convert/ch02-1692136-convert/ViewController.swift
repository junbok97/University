//
//  ViewController.swift
//  ch02-1692136-convert
//
//  Created by 이준복 on 2022/04/25.
//

import UIKit

class ViewController: UIViewController {

    override func viewDidLoad() {
        super.viewDidLoad()
        
        let firstFrame = CGRect(x: 160, y: 240, width: 100, height: 150)
        let firstView = UIView(frame: firstFrame)
        firstView.backgroundColor = .blue
        view.addSubview(firstView)
        
        let secondFrame = CGRect(x: 20, y: 30, width: 50, height: 50)
        let secondView = UIView(frame: secondFrame)
        secondView.backgroundColor = .green
//        view.addSubview(secondView)
        firstView.addSubview(secondView)
        
        
        
        
    }


}

