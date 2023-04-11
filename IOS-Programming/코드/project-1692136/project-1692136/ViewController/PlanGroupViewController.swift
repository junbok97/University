//
//  PlanGroupViewController.swift
//  ch09-1692136-tableView
//
//  Created by 이준복 on 2022/05/01.
//

import UIKit
import FSCalendar

class PlanGroupViewController: UIViewController {
    
    @IBOutlet weak var planGroupTableView: UITableView!
    @IBOutlet weak var fsCalendar: FSCalendar!
    
    var planGroup: PlanGroup!
    var selectedDate: Date? = Date()

    override func viewDidAppear(_ animated: Bool) {
        super.viewDidAppear(animated)
//        Owner.loadOwner(sender: self)
        navigationItem.title = Owner.getOwner()
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        Owner.loadOwner(sender: self)
        
        planGroupTableView.dataSource = self
        planGroupTableView.delegate = self
        planGroupTableView.isEditing = false

        planGroup = PlanGroup(parentNotification: receivingNotification)
        planGroup.queryData(date: Date())

        fsCalendar.dataSource = self
        fsCalendar.delegate = self
        
//        let barButtonItem = UIBarButtonItem(title: "Edit", style: .plain, target: self, action: #selector(editingPlans1))
//        navigationItem.leftBarButtonItem = barButtonItem
        
//        planGroup.plans.sort {
//            $0.date < $1.date
//        }
        
    } // viewDidLoad
    

    

}

extension PlanGroupViewController {
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        if segue.identifier == "ShowPlan" {
            let planDetailViewController = segue.destination as! PlanDetailViewController
            planDetailViewController.saveChangeDelegate = saveChange
            
            if let row = planGroupTableView.indexPathForSelectedRow?.row {
                planDetailViewController.plan = planGroup.getPlans(date: selectedDate)[row].clone()
            }
        }
        
        if segue.identifier == "AddPlan" {
            let planDetailVC = segue.destination as! PlanDetailViewController
            planDetailVC.saveChangeDelegate = saveChange

            planDetailVC.plan = Plan(date: selectedDate, withData: false)
            planGroupTableView.selectRow(at: nil, animated: true, scrollPosition: .none)
        }
    }
    
    func saveChange(plan: Plan?){
        if planGroupTableView.indexPathForSelectedRow != nil {
            planGroup.saveChange(plan: plan!, action: .Modify)
        } else {
            planGroup.saveChange(plan: plan!, action: .Add)
        }
    }
    
    func receivingNotification(plan: Plan?, action: DbAction?) {
        self.planGroupTableView.reloadData()
        fsCalendar.reloadData()
    }
}

extension PlanGroupViewController {
    //    @IBAction func editingPlans(_ sender: UIButton) {
    //        if planGroupTableView.isEditing == true {
    //            planGroupTableView.isEditing = false
    //            sender.setTitle("Edit", for: .normal)
    //        } else {
    //            planGroupTableView.isEditing = true
    //            sender.setTitle("Done", for: .normal)
    //        }
    //    }
    //
    //
    //    @IBAction func addingPlan(_ sender: UIButton) {
    ////        let plan = Plan(date: nil, withData: true)
    ////        planGroup.saveChange(plan: plan, action: .Add)
    ////        performSegue(withIdentifier: "AddPlan", sender: self)
    //
    //        // prepare함수 호출 안함
    //        let planDetailVC = storyboard?.instantiateViewController(withIdentifier: "PlanDetail")
    //        if let vc = planDetailVC {
    ////            present(vc, animated: true, completion: nil)
    //            navigationController?.pushViewController(vc, animated: true)
    //        }
    //    }

    
    @IBAction func editingPlans1(_ sender: UIBarButtonItem) {
        if planGroupTableView.isEditing == true {
            planGroupTableView.isEditing = false
            sender.title = "Edit"
        } else {
            planGroupTableView.isEditing = true
            sender.title = "Done"
        }
    }
    @IBAction func addingPlan1(_ sender: UIBarButtonItem) {
        self.performSegue(withIdentifier: "AddPlan", sender: self)
    }

}

extension PlanGroupViewController: UITableViewDataSource {
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        if let planGroup = planGroup{
            return planGroup.getPlans(date: selectedDate).count
        }
        return 0
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
//        let cell = UITableViewCell(style: .value1, reuseIdentifier: "")
        let cell = tableView.dequeueReusableCell(withIdentifier: "PlanTableViewCell")!
        
        let plan = planGroup.getPlans(date: selectedDate)[indexPath.row]
        
//        cell.textLabel?.text = plan.date.toStringDateTime()
//        cell.detailTextLabel?.text = plan.content
        
        (cell.contentView.subviews[0] as! UILabel).text = plan.date.toStringDateTime()
        (cell.contentView.subviews[1] as! UILabel).text = plan.owner
        (cell.contentView.subviews[2] as! UILabel).text = plan.content
        
//        cell.accessoryType = .none
//        cell.accessoryView = nil
//        if indexPath.row % 2 == 0 {
//            cell.accessoryType = .detailDisclosureButton
//        } else {
//            cell.accessoryView = UISwitch(frame: CGRect())
//        }
        
        return cell
    }
}

extension PlanGroupViewController: UITableViewDelegate {
    
//    func tableView(_ tableView: UITableView, commit editingStyle: UITableViewCell.EditingStyle, forRowAt indexPath: IndexPath) {
//
//        if editingStyle == .delete {
//            let plan = self.planGroup.getPlans()[indexPath.row]
//            self.planGroup.saveChange(plan: plan, action: .Delete)
//        }
//    }
    
    
    
    func tableView(_ tableView: UITableView, commit editingStyle: UITableViewCell.EditingStyle, forRowAt indexPath: IndexPath) {
        if editingStyle == .delete {
            let plan = self.planGroup.getPlans(date: selectedDate)[indexPath.row]
            let title = "Delete \(plan.content)"
            let message = "Are you sure you want to delete this item?"
            
            let alertController = UIAlertController(title: title, message: message, preferredStyle: .alert)
            let cancelAction = UIAlertAction(title: "Cancel", style: .cancel, handler: nil)
            let deleteAction = UIAlertAction(title: "Delete", style: .destructive) { (action: UIAlertAction) -> Void in
                let plan = self.planGroup.getPlans(date: self.selectedDate)[indexPath.row]
                self.planGroup.saveChange(plan: plan, action: .Delete)
            }
            
            alertController.addAction(cancelAction)
            alertController.addAction(deleteAction)
            present(alertController, animated: true, completion: nil)
        }
    }
    
    func tableView(_ tableView: UITableView, moveRowAt sourceIndexPath: IndexPath, to destinationIndexPath: IndexPath) {
        let from = planGroup.getPlans(date: selectedDate)[sourceIndexPath.row]
        let to = planGroup.getPlans(date: selectedDate)[destinationIndexPath.row]
        planGroup.changePlan(from: from, to: to)
        tableView.moveRow(at: sourceIndexPath, to: destinationIndexPath)
    }
}

extension PlanGroupViewController: FSCalendarDelegate, FSCalendarDataSource {
    
    func calendar(_ calendar: FSCalendar, didSelect date: Date, at monthPosition: FSCalendarMonthPosition) {
        selectedDate = date
        planGroup.queryData(date: date)
        planGroupTableView.reloadData()
    }
    
    func calendarCurrentPageDidChange(_ calendar: FSCalendar) {
        selectedDate = calendar.currentPage
        planGroup.queryData(date: calendar.currentPage)
    }
    
    func calendar(_ calendar: FSCalendar, subtitleFor date: Date) -> String? {
        let plans = planGroup.getPlans(date: date)
        if plans.count > 0 {
            return "[\(plans.count)]"
        }
        return nil
    }
    
    
}
