//
//  PlanGroupViewController.swift
//  ch09-1692136-tableView
//
//  Created by 이준복 on 2022/05/01.
//

import UIKit

class PlanGroupViewController: UIViewController {

    @IBOutlet weak var planGroupTableView: UITableView!
    var planGroup: PlanGroup!
    
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        if segue.identifier == "ShowPlan" {
            let planDetailViewController = segue.destination as! PlanDetailViewController
            planDetailViewController.saveChangeDelegate = saveChange
            
            if let row = planGroupTableView.indexPathForSelectedRow?.row {
                planDetailViewController.plan = planGroup.getPlans()[row].clone()
            }
            
        }
    }
    override func viewDidAppear(_ animated: Bool) {
        Owner.loadOwner(sender: self)
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        Owner.loadOwner(sender: self)
        planGroupTableView.dataSource = self
        planGroupTableView.delegate = self
        planGroupTableView.isEditing = false
        
        planGroup = PlanGroup(parentNotification: receivingNotification)
        planGroup.queryData(date: Date())
    } // viewDidLoad
    
    
    func saveChange(plan: Plan?){
        if planGroupTableView.indexPathForSelectedRow != nil {
            planGroup.saveChange(plan: plan!, action: .Modify)
        } else {
            planGroup.saveChange(plan: plan!, action: .Add)
        }
    }
    
    
    func receivingNotification(plan: Plan?, action: DbAction?) {
        self.planGroupTableView.reloadData()
    }
    
    @IBAction func editingPlans(_ sender: UIButton) {
        if planGroupTableView.isEditing == true {
            planGroupTableView.isEditing = false
            sender.setTitle("Edit", for: .normal)
        } else {
            planGroupTableView.isEditing = true
            sender.setTitle("Done", for: .normal)
        }
    }
    
    @IBAction func addingPlan(_ sender: UIButton) {
        let plan = Plan(date: nil, withData: true)
        planGroup.saveChange(plan: plan, action: .Add)
    }
    


}

extension PlanGroupViewController: UITableViewDataSource {
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        if let planGroup = planGroup{
            return planGroup.getPlans().count
        }
        return 0
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
//        let cell = UITableViewCell(style: .value1, reuseIdentifier: "")
        let cell = tableView.dequeueReusableCell(withIdentifier: "PlanTableViewCell")!
        
        let plan = planGroup.getPlans()[indexPath.row]
        
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
//
//            self.planGroup.saveChange(plan: plan, action: .Delete)
//        }
//    }
    
    func tableView(_ tableView: UITableView, commit editingStyle: UITableViewCell.EditingStyle, forRowAt indexPath: IndexPath) {
        if editingStyle == .delete {
            let plan = self.planGroup.getPlans()[indexPath.row]
            let title = "Delete \(plan.content)"
            let message = "Are you sure you want to delete this item?"
            
            let alertController = UIAlertController(title: title, message: message, preferredStyle: .alert)
            let cancelAction = UIAlertAction(title: "Cancel", style: .cancel, handler: nil)
            let deleteAction = UIAlertAction(title: "Delete", style: .destructive) { (action: UIAlertAction) -> Void in
                let plan = self.planGroup.getPlans()[indexPath.row]
                self.planGroup.saveChange(plan: plan, action: .Delete)
            }
            
            alertController.addAction(cancelAction)
            alertController.addAction(deleteAction)
            present(alertController, animated: true, completion: nil)
        }
    }
    
    func tableView(_ tableView: UITableView, moveRowAt sourceIndexPath: IndexPath, to destinationIndexPath: IndexPath) {
        let from = planGroup.getPlans()[sourceIndexPath.row]
        let to = planGroup.getPlans()[destinationIndexPath.row]
        planGroup.changePlan(from: from, to: to)
        tableView.moveRow(at: sourceIndexPath, to: destinationIndexPath)
    }
}
