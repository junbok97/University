//
//  MemoGroup.swift
//  ch13-AlbumWithMemo
//
//  Created by 이준복 on 2022/06/13.
//

import Foundation

class MemoGroup{
    
    let memoUrl: URL = FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).first!
    var memos: [String: String]!
    
    init(){
        let photoMemosDir = memoUrl.appendingPathComponent("Photos/Momos")
        try? FileManager.default.createDirectory(atPath: photoMemosDir.path, withIntermediateDirectories: true, attributes: nil)
        
        memos = [:]
        let unarchived = try? Data(contentsOf: photoMemosDir.appendingPathComponent("memos.archive"))//읽어온다
        if let unarchived = unarchived{
            memos = try? NSKeyedUnarchiver.unarchiveTopLevelObjectWithData(unarchived) as? [String: String] // 복원한다
        }
    }
    
    func saveMemoGroup(){
        guard let memos = memos else{ return }
        let photoMemosDir = memoUrl.appendingPathComponent("Photos/Momos")
        try? FileManager.default.createDirectory(atPath: photoMemosDir.path, withIntermediateDirectories: true, attributes: nil)
        let archived = try? NSKeyedArchiver.archivedData(withRootObject: memos, requiringSecureCoding: false) // 압축한다
        try? archived?.write(to: photoMemosDir.appendingPathComponent("memos.archive")) // 저장한다
    }
    
    func getMemo(key: String) -> String?{
        return memos?[key]
    }
    
    func putMemo(key: String, memo: String){
        memos?[key] = memo
    }
}
