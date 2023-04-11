//
//  ViewController.swift
//  ch13-AlbumWithMemo
//
//  Created by 이준복 on 2022/06/11.
//

import UIKit
import Photos

class AlbumViewController: UIViewController {

    @IBOutlet weak var collectionView: UICollectionView!
    
    var fetchResult: PHFetchResult<PHAsset>!    // 사진에 대한 메타 데이터 저장
    var memoGroup = MemoGroup()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        navigationItem.title = "앨범관리"
        collectionView.delegate = self        // 테이블뷰와 완전 동일하다
        collectionView.dataSource = self      // 테이블뷰와 완전 동일하다
    } // viewDidLoad
    
    override func viewWillAppear(_ animated: Bool) {
        // 모든 사진을 다 가져온다. 일부사진만 가져오는 것은
        // https://developer.apple.com/documentation/photokit/browsing_and_modifying_photo_albums 참조
        let allPhotosOptions = PHFetchOptions()
        allPhotosOptions.sortDescriptors = [NSSortDescriptor(key: "creationDate", ascending: false)]
        fetchResult = PHAsset.fetchAssets(with: allPhotosOptions) // 모든 사진의 목록을 갖는다
        collectionView.reloadData()
    } // viewWillAppear
    
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        
        let albumDetailViewController = segue.destination as! AlbumDetailViewController

        // 이미지에 대한 정보를 가져온다
        let indexPath = sender as! IndexPath    // sender이 indexPath이다.
        let asset = fetchResult.object(at: indexPath.row)
        albumDetailViewController.memoIdentifier = asset.localIdentifier  // 이미지에 대한 식별자이다.
        albumDetailViewController.memoGroup = memoGroup

        let options = PHImageRequestOptions()
        options.deliveryMode = .highQualityFormat // 고해상도를 가져오기 우l함임
        PHCachingImageManager.default().requestImage(for: asset, targetSize: CGSize(), contentMode: .aspectFill, options: options, resultHandler: { image, _ in
            // 한참있다가 실행된다. 즉, albumDetailViewController가 로딩되고 appear한 후에 나타난다.
            albumDetailViewController.image = image  // 앞에서 didSet을 사용한 이유이다.
        }) // PHCachingImageManager
    } // prepare
    
    @IBAction func takePicture(_ sender: Any) {
        let pickerViewController = UIImagePickerController()
        if UIImagePickerController.isSourceTypeAvailable(.camera) == true{
            pickerViewController.sourceType = .camera
        }else{
            pickerViewController.sourceType = .photoLibrary
        }
        pickerViewController.delegate = self
        present(pickerViewController, animated: true, completion: nil)

    }
    
} // AlbumViewController

extension AlbumViewController: UICollectionViewDelegate, UICollectionViewDataSource, UICollectionViewDelegateFlowLayout{
    func collectionView(_ collectionView: UICollectionView, numberOfItemsInSection section: Int) -> Int {
        // 사진의 갯수를 리턴한다.
        return fetchResult == nil ? 0: fetchResult.count
    } // numberOfItemsInSection

    func collectionView(_ collectionView: UICollectionView, cellForItemAt indexPath: IndexPath) -> UICollectionViewCell {

        let cell = collectionView.dequeueReusableCell(withReuseIdentifier: "ImageCollectionViewCell", for:  indexPath) as! ImageCollectionViewCell
        
        let asset = fetchResult.object(at: indexPath.row)  // 이미지에 대한 메타 데이터를 가져온다
        PHCachingImageManager.default().requestImage(for: asset, targetSize: CGSize(), contentMode: .aspectFill, options: nil){
            (image, _) in    // 요청한 이미지를 디스크로부터 읽으면 이 함수가 호출 된다.
            cell.imageView.image = image  // 여기서 이미지를 보이게 한다
            cell.memoLabel.text = self.memoGroup.getMemo(key: asset.localIdentifier) ?? "메모없음"
        } // PHCachingImageManager
        return cell
    }// cellForItemAt
    
    func collectionView(_ collectionView: UICollectionView, layout collectionViewLayout: UICollectionViewLayout, sizeForItemAt indexPath: IndexPath) -> CGSize {
        // CollectionView에 하나의 이미지의 크기를 리턴한다.
        // indexPath에 따라 크리를 조정하는 것도 가능하다.
       return CGSize(width: 110, height: 110)
    } // collectionViewLayout
    
    func collectionView(_ collectionView: UICollectionView, didSelectItemAt indexPath: IndexPath) {
        // 이 이미지를 클릭하면 자세히 보기로 전이한다. Send가 self가 아니고 클릭된 Cell의 indexPath이다.
        performSegue(withIdentifier: "ShowDetail", sender: indexPath)
    } // didSelectItemAt
    
} // extension AlbumViewController: UICollectionViewDelegate, UICollectionViewDataSource


extension AlbumViewController: UIImagePickerControllerDelegate, UINavigationControllerDelegate{
    
    func imagePickerControllerDidCancel(_ picker: UIImagePickerController) {
        picker.dismiss(animated: true, completion: nil)
    } // imagePickerControllerDidCancel

    func imagePickerController(_ picker: UIImagePickerController, didFinishPickingMediaWithInfo info: [UIImagePickerController.InfoKey : Any]) {
        // 이미지를 가져온다
        let image = info[UIImagePickerController.InfoKey.originalImage] as! UIImage
        picker.dismiss(animated: true, completion: nil)

        // 앨범에 저장을 요청하면서 저장 완료에 대한 handler를 제공한다
        UIImageWriteToSavedPhotosAlbum(image, self, #selector(afterSaveImage), nil)
    } // didFinishPickingMediaWithInfo
} // extension AlbumViewController: UIImagePickerControllerDelegate, UINavigationControllerDelegate

// @objc
extension AlbumViewController{
    @objc func afterSaveImage(_ image: UIImage, didFinishSavingWithError error: NSError?, contextInfo: UnsafeRawPointer) {

        // 새로운 사진을 포함하여 모든 사진의 메타 데이터를 가져온다
        let allPhotosOptions = PHFetchOptions()
        allPhotosOptions.sortDescriptors = [NSSortDescriptor(key: "creationDate", ascending: false)]
        fetchResult = PHAsset.fetchAssets(with: allPhotosOptions)

        // 새로운 사진은 맨처음에 존재한다. 왜냐하면 생성날자순으로 정렬하였기 때문에
        let indexPath = IndexPath(row: 0, section: 0)  // 첫 사진에 대한 indexPath 설정
        performSegue(withIdentifier: "ShowDetail", sender: indexPath) // 그러면 prepare가 호출 될 것이다.
    } // @objc func afterSaveImage
} // extemsion AlbumViewController
