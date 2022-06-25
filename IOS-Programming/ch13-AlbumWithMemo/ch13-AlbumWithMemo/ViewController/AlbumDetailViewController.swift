//
//  AlbumDetailViewController.swift
//  ch13-AlbumWithMemo
//
//  Created by 이준복 on 2022/06/13.
//

import UIKit

class AlbumDetailViewController: UIViewController {

    @IBOutlet weak var imageView: UIImageView!
    @IBOutlet weak var memoTextView: UITextView!
    @IBOutlet weak var stackView: UIStackView!
    @IBOutlet weak var bottomStackViewConstraint: NSLayoutConstraint!
    
    var image: UIImage? { // 이미지 객체를 전달받는 변수임
        didSet {     // image값이 변경되면 항상 함수 didSet가 호출된다
     if let imageView = imageView{  // imageView가 만들어지기 전에 호출 될수도 있다
                           imageView.image = image
                 }
        }
    }

    var memoIdentifier: String! // 이미지 식별자를 전달 받음
    var memoGroup: MemoGroup!   // 메모 그룹을 전달 받음

    override func viewDidLoad() {
        super.viewDidLoad()
        imageView.image = image ?? nil  // nil인 경우는 위의 didSet가 imageView가 생성되기 후에 호출된 경우
        memoTextView.text = memoGroup.getMemo(key: memoIdentifier)
        let tapGestureRecognizer = UITapGestureRecognizer(target: self, action: #selector(removeKeyboard))
        view.addGestureRecognizer(tapGestureRecognizer)
        
    } // ViewDidLoad
    
    override func viewWillAppear(_ animated: Bool) {
        // 키보드가 나타나면 keyboardWillShow 함수를 호출한다
        NotificationCenter.default.addObserver(self,selector: #selector(keyboardWillShow),
          name: UIResponder.keyboardWillShowNotification, object: nil
        )
        // 키보드가 사라지면 keyboardWillHide 함수를 호출한다
        NotificationCenter.default.addObserver(self,selector: #selector(keyboardWillHide),
          name: UIResponder.keyboardWillHideNotification,object: nil
        )
    } // viewWillAppear
    
    
    override func viewWillDisappear(_ animated: Bool) {
        NotificationCenter.default.removeObserver(self)
        memoGroup.memos[memoIdentifier] = memoTextView.text
        memoGroup.saveMemoGroup()

    } // viewWillDisappear
    
} // class AlbumDetailViewController

// objc
extension AlbumDetailViewController{
    
    @objc private func keyboardWillShow(_ notification: Notification) {
        if let keyboardFrame: NSValue = notification.userInfo?[UIResponder.keyboardFrameEndUserInfoKey] as? NSValue {
            let keyboardHeight = keyboardFrame.cgRectValue.height  // 키보드의 높이를 구한다
            bottomStackViewConstraint.constant = keyboardHeight   // 스택뷰의 하단 여백을 높인다
        } // if let keyboardFrame
    }
    
    @objc private func keyboardWillHide(_ notification: Notification) {
        bottomStackViewConstraint.constant = 8     // 스택뷰의 하단 여백을 원래대로 설정한다
    } // keyboardWillHide
    
    @objc func removeKeyboard(sender: UITapGestureRecognizer){
        memoTextView.resignFirstResponder()
    } // removeKeyboard
    
} // extension AlbumViewController
