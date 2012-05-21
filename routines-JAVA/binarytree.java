/* Balanced binary deletion tree */
/* Accepted: UVA 10909, UVA 11525 (simpler version) */

class BDTree {
	public Node root;

	class Node {
		Node left, right;
		int cleft, cright, val;
		boolean has;
		Node(int b) {
			cleft = cright = 0;
			val = b;
			left = right = null;
			has = true;
		}
	}

	/* create tree from array, sorted on INDEX, not content, O(N) */
	BDTree(int a[]) { root = build(a, 0, a.length-1); }
	Node build(int a[], int lo, int hi) {
		int mid = (lo+hi)/2;
		Node node = new Node(a[mid]);
		if(mid > lo) {
			node.left = build(a, lo, mid-1);
			node.cleft = mid-lo;
		}
		if(mid < hi) {
			node.right = build(a, mid+1, hi);
			node.cright = hi-mid;
		}
		return node;
	}

	/* returns index for element with value */
	/* only works if the tree was build from a sorted array! */
	/* TODO test this routine */
	int getindex(int val) { return getindex(root,val); }
	int getindex(Node node, int val) {
		if(val<node.val && node.left==null) return -1;
		if(val>node.val && node.right==null) return -1;
		if(val==node.val) return node.has?node.cleft:-1;
		else if(val<node.val) return getindex(node.left,val);
		else return node.cleft+(node.has?1:0)+getindex(node.right,val);
	}

	/* get and mark as removed nth element (0-indexed), O(log N) */
	int extractnth(int index) { return extractnth(root,index); }
	int extractnth(Node node,int index) {
		if(index == node.cleft && node.has) {
			node.has = false;
			return node.val;
		} else if(index < node.cleft) {
			node.cleft--;
			return extractnth(node.left, index);
		} else {
			node.cright--;
			return extractnth(node.right, index - node.cleft - (node.has?1:0));
		}
	}

	/* get and don't remove nth element (0-indexed), O(log N) */
	int getnth(int index) { return getnth(root,index); }
	int getnth(Node node,int index) {
		if(index == node.cleft && node.has) return node.val;
		else if(index < node.cleft) return getnth(node.left, index);
		else return getnth(node.right, index - node.cleft - (node.has?1:0));
	}

	/* create array from tree, O(N) */
	int[] toArray() {
		ix = 0;
		int ret[] = new int[size()];
		toarray(ret, root);
		return ret;
	}

	int ix;
	void toarray(int ret[],Node node) {
		if(node.cleft > 0) toarray(ret, node.left);
		if(node.has) ret[ix++] = node.val;
		if(node.cright > 0) toarray(ret, node.right);
	}

	/* number of non-removed elements, O(1) */
	int size() { return root.cleft + root.cright + (root.has?1:0); }
}
