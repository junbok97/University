//
//  Plan.swift
//  ch09-1692136-tableView
//
//  Created by 이준복 on 2022/05/01.
//

import Foundation

class Plan: NSObject, NSCoding {
    
    enum Kind: Int {
        case Todo = 0, Metting, Study, Etc
        func toString() -> String {
            switch self {
                case .Todo: return "할일"
                case .Metting: return "미팅"
                case .Study: return "공부"
                case .Etc : return "기타"
            } // switch self
        } // func toString()
        static var count: Int {return Kind.Etc.rawValue + 1}
    } // enum Kind
    
    var key, content: String
    var owner: String?
    var date: Date
    var kind: Kind
    
    init(date: Date, kind: Kind, content: String) {
        self.key = UUID().uuidString
        self.date = Date(timeInterval: 0, since: date)
        self.owner = Owner.getOwner()
        self.kind = kind
        self.content = content
        super.init()
    } // init()
    
    // archiving할때 호출된다
    func encode(with aCoder: NSCoder) {
        aCoder.encode(key, forKey: "key")       // 내부적으로 String의 encode가 호출된다
        aCoder.encode(date, forKey: "date")
        aCoder.encode(owner, forKey: "owner")
        aCoder.encode(kind.rawValue, forKey: "kind")
        aCoder.encode(content, forKey: "content")
    }
    // unarchiving할때 호출된다
    required init(coder aDecoder: NSCoder) {
        key = aDecoder.decodeObject(forKey: "key") as! String? ?? "" // 내부적으로 String.init가 호출된다
        date = aDecoder.decodeObject(forKey: "date") as! Date
        owner = aDecoder.decodeObject(forKey: "owner") as? String
        let rawValue = aDecoder.decodeInteger(forKey: "kind")
        kind = Kind(rawValue: rawValue)!
        content = aDecoder.decodeObject(forKey: "content") as! String? ?? ""
        super.init()
    }

} // class Plan

extension Plan {
    convenience init(date: Date? = nil, withData: Bool = false) {
        if withData == true {
            var index = Int(arc4random_uniform(UInt32(Kind.count)))
            let kind = Kind(rawValue: index)!
            
            let contents = ["IOS 숙제", "졸업 프로젝트", "아르바이트", "데이트", "엄마 도와드리기"]
            index = Int(arc4random_uniform(UInt32(contents.count)))
            let content = contents[index]
            
            self.init(date: date ?? Date(), kind: kind, content: content)
        } else {
            self.init(date: date ?? Date(), kind: .Etc, content: "")
        } // if withData else
    } // convenience init
} // extension Plan

extension Plan {
    func clone() -> Plan{
        let clonee = Plan()
        
        clonee.key = self.key
        clonee.date = Date(timeInterval: 0, since: self.date)
        clonee.owner = Owner.getOwner()
        clonee.kind = self.kind
        clonee.content = self.content
        return clonee
    }
}

