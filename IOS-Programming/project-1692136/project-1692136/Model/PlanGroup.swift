//
//  PlanGroup.swift
//  ch09-1692136-tableView
//
//  Created by 이준복 on 2022/05/01.
//

import Foundation

class PlanGroup: NSObject {
    
    var plans = [Plan]() {
        didSet{
            plans.sort {
                $0.date < $1.date
            }
        }
    }
    var fromDate, toDate: Date?
    var database: Database!
    var parentNotification: ((Plan?, DbAction?) -> Void)?
    
    init(parentNotification: ((Plan?, DbAction?) -> Void)? ) {
        super.init()
        self.parentNotification = parentNotification
//        database = DbMemory(parentNotification: receivingNotification)
//        database = DbFile(parentNotification: receivingNotification) // 데이터베이스 생성
        database = DbFirebase(parentNotification: receivingNotification) // 데이터베이스 생성
    } // init
} // class PlanGroup

extension PlanGroup {
    private func count() -> Int {return plans.count}
    private func addPlan(plan: Plan) {plans.append(plan)}
    private func modifyPlan(modifiedPlan: Plan) {
        if let index = find(modifiedPlan.key) {
            plans[index] = modifiedPlan
        }
    } // private func modifyPlan
    private func removePlan(removedPlan: Plan) {
        if let index = find(removedPlan.key) {
            plans.remove(at: index)
        }
    } // private func removedPlan
    func changePlan(from: Plan, to: Plan){
        if let fromIndex = find(from.key), let toIndex = find(to.key) {
            plans[fromIndex] = to
            plans[toIndex] = from
        } // if let
    } // func changePlan
}

extension PlanGroup {
    func receivingNotification(plan: Plan?, action: DbAction?) {
        if let plan = plan {
            switch action {
            case .Add: addPlan(plan: plan)
            case .Modify: modifyPlan(modifiedPlan: plan)
            case .Delete: removePlan(removedPlan: plan)
            default:
                break
            } // switch action
        } // if let plan
        
        if let parentNotification = parentNotification {
            parentNotification(plan, action)
        } // if let parentNotification
    } // func receivingNotification
}

extension PlanGroup {
    
    func queryData(date: Date) {
        // 새로운 쿼리에 맞는 데이터를 채우기 위해 기존 데이터를 전부 지운디
        plans.removeAll()
        
        // date가 속한 1개월 +- 알파만큼 가져온다
        fromDate = date.firstOfMonth().firstOfWeek() // 1일이 속한 일요일을 시작시간
        toDate = date.lastOfMonth().lastOfWeek() // 이달 마지막일이 속한 토요일을 마감시간
        database.queryPlan(fromDate: fromDate!, toDate: toDate!)
    } // func queryData
    
    func saveChange(plan: Plan, action: DbAction) {
        // 단순히 데이터베이스에 변경요청을하고 plans에 대해서는
        // 데이터베이스가 변경알림을 호출하는 receivingNotification에서 적용한다
        database.saveChange(plan: plan, action: action)
    } // func saveChange
    
    func getPlans(date: Date? = nil) -> [Plan] {
        // plans중에서 date날짜에 있는 것만 리턴
        if let date = date {
            var planForDate: [Plan] = []
            let start = date.firstOfDay()
            let end = date.lastOfDay()
            for plan in plans {
                if plan.date >= start && plan.date <= end {
                    planForDate.append(plan)
                } // if plan.date
            } // for plan
            return planForDate
        } // if let date
        return plans
    } // func getPlans

    func isln(date: Date) -> Bool {
        if let from = fromDate, let to = toDate {
            return (date >= from && date <= to) ? true : false
        } // if let
        return false
    } // func isln
    
    private func find(_ key: String) -> Int? {
        for i in 0..<plans.count{
            if key == plans[i].key {
                return i
            } // if key
        } // for
        return nil
    } // private func find
}
